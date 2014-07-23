require 'cgi'
require 'json'
require 'dropbox_sdk'
require 'rss'
require 'open-uri'
require 'socket'

APP_KEY     = '3743du3zuz8gdy8'
APP_SECRET  = '3zq6w82srafqqul'
CONFIG = {
	uuid:      6274093,
	token:     'pUjeEsNnW_oAAAAAAAAIZgrs6QRP4dro9GdVD1nO6D_1Vq6qP2LIlcMMakMOva_4',
	directory: '/ShaderPapiere',
	rss:       'https://www.dropbox.com/6274093/503875465/4xqSA4KOGxNS5vFDG5He7w3EQrzdQfsBakUmRDvQ/events.xml',
	server:    'irc.ps0ke.de',
	port:      2342,
	room:      '#rtificial',
	nick:      'Dropbox',
	message_without_join: true,
	colors:    true,
	notice:    true,
	event_url: false,
}

cgi = CGI.new

if cgi.request_method == 'GET'
	puts cgi.header(type: 'text/plain')
	print cgi['challenge']
	exit
end

if cgi.request_method == 'POST'
	puts cgi.header(type: 'text/plain')
	data = JSON.parse cgi.params.keys.first
	data['delta']['users'].each do |uuid|
		next unless uuid == CONFIG[:uuid]
		client = DropboxClient.new(CONFIG[:token])
		user_name = client.account_info['display_name']

		feed = RSS::Parser.parse(open(CONFIG[:rss]))

		cursor_file_name = ".db-irc-notify.#{uuid}.cursor"
		File.open(cursor_file_name, "w"){} unless File.exists? cursor_file_name

		has_more = true
		while has_more do
			cursor = String.new
			File.open(cursor_file_name, 'r'){ |f| cursor = f.read }
			cursor = nil if cursor.empty?

			delta = client.delta(cursor, CONFIG[:directory])
			File.open(cursor_file_name, 'w'){ |f| f.write delta['cursor'] }

			delta['entries'].each do |entry|
				if entry[1].nil? # got deleted
					puts entry.first
					puts "<deleted>"

					irc = TCPSocket.open(CONFIG[:server], CONFIG[:port])
					irc.print "NICK #{CONFIG[:nick]}\r\n"
					irc.print "USER #{CONFIG[:nick]} #{cgi.server_name} #{CONFIG[:nick]} :#{CONFIG[:nick]}\r\n"
					irc_command = CONFIG[:notice] ? 'NOTICE' : 'PRIVMSG'
					irc.print "JOIN #{CONFIG[:room]}\r\n" unless CONFIG[:message_without_join]

					irc.print "#{irc_command} #{CONFIG[:room]} :[#{CONFIG[:colors] ? "\x02\x0306" : ''}#{File.basename CONFIG[:directory]}#{CONFIG[:colors] ? "\x03\x02" : ''}] #{CONFIG[:colors] ? "\x02\x0304" : ''}Deleted#{CONFIG[:colors] ? "\x03\x02" : ''}: #{CONFIG[:colors] ? "\x0306" : ''}#{File.basename(entry.first)}#{CONFIG[:colors] ? "\x03" : ''}\r\n"

					irc.print "QUIT\r\n"
					irc.close
					next
				end
				matching_item = nil
				feed.items.each do |item|
					matching_item = item if item.pubDate == Time.parse(entry[1]['modified'])
				end
				if matching_item
					info = matching_item.description.match(/In <.+>, (.+) (edited|added|deleted) the \w+ <a target='_blank' href='(.+)'>/).captures
					who = info[0]
					who = user_name if who == 'you'
					how = info[1]
					link = info[2]

					puts "-#{CONFIG[:nick]}:#{CONFIG[:room]}- [#{File.basename CONFIG[:directory]}] #{who} #{how} #{File.basename(entry[1]['path'])}: http://dropbox.com#{link}"
					puts "-#{CONFIG[:nick]}:#{CONFIG[:room]}- #{entry[1]['path']} #{entry[1]['size']} #{entry[1]['mime_type']}"

					irc = TCPSocket.open(CONFIG[:server], CONFIG[:port])
					irc.print "NICK #{CONFIG[:nick]}\r\n"
					irc.print "USER #{CONFIG[:nick]} #{cgi.server_name} #{CONFIG[:nick]} :#{CONFIG[:nick]}\r\n"
					irc_command = CONFIG[:notice] ? 'NOTICE' : 'PRIVMSG'
					irc.print "JOIN #{CONFIG[:room]}\r\n" unless CONFIG[:message_without_join]

					irc.print "#{irc_command} #{CONFIG[:room]} :[#{CONFIG[:colors] ? "\x02\x0306" : ''}#{File.basename CONFIG[:directory]}#{CONFIG[:colors] ? "\x03\x02" : ''}] #{who} #{how} #{CONFIG[:colors] ? "\x0306" : ''}#{File.basename(entry[1]['path'])}#{CONFIG[:colors] ? "\x03" : ''}" + (CONFIG[:event_url] ? ": #{CONFIG[:colors] ? "\x1F\x0302" : ''}http://dropbox.com#{link}#{CONFIG[:colors] ? "\x03\x1F" : ''}\r\n" : "\r\n")
					irc.print "#{irc_command} #{CONFIG[:room]} :#{CONFIG[:colors] ? "\x0306" : ''}#{entry[1]['path']}#{CONFIG[:colors] ? "\x03" : ''} #{CONFIG[:colors] ? "\x02\x0314" : ''}#{entry[1]['size']}#{CONFIG[:colors] ? "\x03\x02" : ''} #{entry[1]['mime_type']}\r\n"

					irc.print "QUIT\r\n"
					irc.close
				end
			end
			has_more = delta['has_more']
		end
	end
end
