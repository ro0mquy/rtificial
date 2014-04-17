ARGV.each do |hexcolor|
	print "vec3("
	hexcolor.match(/#?([0-9a-f]{2})([0-9a-f]{2})([0-9a-f]{2})/i) do |match|
		print match.captures.map { |channel|
			channel.to_s.hex/255.0
		}.join(', ')
	end
	print ");\n"
end
