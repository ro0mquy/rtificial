#!/bin/env ruby
require 'json'
require 'socket'
require 'open-uri'
require 'nokogiri'

CONFIG = {
	cursor:       "#{Dir.home}/.pouet.cursor",
	server:       'irc.ps0ke.de',
	port:         2342,
	room:         '#rtificial',
	nick:         'pouet',
	message_without_join: true,
	colors:       true,
	notice:       true,
	prod_url:     'http://pouet.net/prod.php?which=', # set to false to not post the link
	unicode:      false,
	groups:       [12306, 12651],
}

class Irc
	def session
		@irc = TCPSocket.open(CONFIG[:server], CONFIG[:port])
		#@irc = $stdout
		@irc.print "NICK #{CONFIG[:nick]}\r\n"
		@irc.print "USER #{CONFIG[:nick]} #{`hostname`.chomp} #{CONFIG[:nick]} :#{CONFIG[:nick]}\r\n"
		@irc_command = CONFIG[:notice] ? 'NOTICE' : 'PRIVMSG'
		@irc.print "JOIN #{CONFIG[:room]}\r\n" unless CONFIG[:message_without_join]

		yield self

		@irc.print "QUIT\r\n"
		@irc.close
	end

	def announce(group, num_comments, prod, prod_id)
		@irc.print (("#{@irc_command} #{CONFIG[:room]} :[#{CONFIG[:colors] ? "\x0306" : ''}#{group}#{CONFIG[:colors] ? "\x03" : ''}] #{CONFIG[:colors] ? "\x02" : ''}#{num_comments}#{CONFIG[:colors] ? "\x02" : ''} new comment#{"s" if num_comments > 1} on #{CONFIG[:colors] ? "\x0306" : ''}#{prod}#{CONFIG[:colors] ? "\x03" : ''}" + (CONFIG[:prod_url] ? ": #{CONFIG[:colors] ? "\x1F\x0302" : ''}#{CONFIG[:prod_url] + prod_id.to_s}#{CONFIG[:colors] ? "\x03\x1F" : ''}" : "")).delete("\r\n") + "\r\n")
	end

	def comment(who, rating, comment)
		if CONFIG[:unicode]
			rating = case rating
					 when 1
						 "👍"
					 when 0
						 "🐷"
					 when -1
						 "👎"
					 end
		else
			rating = case rating
					 when 1
						 "+"
					 when 0
						 "~"
					 when -1
						 "-"
					 end
		end
		@irc.print (("#{@irc_command} #{CONFIG[:room]} :#{CONFIG[:colors] ? "\x0306" : ''}#{who}#{CONFIG[:colors] ? "\x03" : ''} #{CONFIG[:colors] ? "\x02\x0314" : ''}#{rating}#{CONFIG[:colors] ? "\x03\x02" : ''} #{comment}").delete("\r\n") + "\r\n")
	end
end

prods = Array.new

CONFIG[:groups].each do |group|
	group = group.to_s
	page = Nokogiri::HTML(open('http://pouet.net/groups.php?which=' + group))
	the_prods = page.css("span.prod")
	the_prods.each do |prod|
		url = prod.css("a").first[:href]
		prod_id = /\d+$/.match(url)
		prods.push  prod_id
	end

end

if File.exists? CONFIG[:cursor]
	cursor = JSON.parse(File.read(CONFIG[:cursor]))
else
	cursor = Hash.new
end

prods.each do |prod|
	prod = prod.to_s
	page = Nokogiri::HTML(open(CONFIG[:prod_url] + prod))

	info = page.css("#title")
	title = info.css("big").first.text
	group = info.css("a").first.text

	newest_known = cursor.has_key?(prod) ? DateTime.parse(cursor[prod]) : DateTime.new(1970, 1, 1)
	new_comments = Array.new

	comments = page.css("#pouetbox_prodcomments div.comment")
	comments.each do |comment|
		content = comment.css(".content").first.text

		footer = comment.css(".foot").first.text
		/^(?<rating>|rulez|sucks) added on the (?<date>.+) by (?<author>.+)$/ =~ footer
		rating = case rating
				 when 'rulez'
					 1
				 when ''
					 0
				 when 'sucks'
					 -1
				 end
		date = DateTime.parse(date)
		author.strip!

		if date > newest_known
			new_comments.push({author: author, date: date, rating: rating, content: content})
			newest_known = date
		end
	end

	cursor[prod] = newest_known.to_s

	if new_comments.size > 0
		Irc.new.session do |irc|
			irc.announce(group, new_comments.size, title, prod)

			new_comments.each do |c|
				irc.comment(c[:author], c[:rating], c[:content])
			end
		end
	end
end

File.write(CONFIG[:cursor], JSON.generate(cursor))
