require 'json'

if ARGV.count != 2
	puts "usage: convert.rb scene-name path/to/timeline.json"
	exit 1
end

scene = ARGV.first
timeline_json = JSON.parse( File.open(ARGV[1], 'r').read )
keyframes_h = File.open(scene+'_keyframes.h', 'w')
num_keyframes = timeline_json['keyframes'].count

keyframes_h.puts "static keyframe_list_t #{scene}_keyframe_list = {"
keyframes_h.puts "\t#{num_keyframes},"
keyframes_h.puts "\t#{num_keyframes},"
keyframes_h.puts "\t{"

timeline_json['keyframes'].each_with_index do |keyframe, i|
	keyframes_h.puts "\t\t{"
	keyframes_h.puts "\t\t\t#{keyframe['time'].to_i},"
	keyframes_h.puts "\t\t\t{"
	keyframes_h.puts "\t\t\t\t{"
	keyframes_h.puts "\t\t\t\t\t#{keyframe['position'][0]},"
	keyframes_h.puts "\t\t\t\t\t#{keyframe['position'][1]},"
	keyframes_h.puts "\t\t\t\t\t#{keyframe['position'][2]}"
	keyframes_h.puts "\t\t\t\t},"
	keyframes_h.puts "\t\t\t\t{"
	keyframes_h.puts "\t\t\t\t\t{"
	keyframes_h.puts "\t\t\t\t\t\t#{keyframe['rotation'][0]},"
	keyframes_h.puts "\t\t\t\t\t\t#{keyframe['rotation'][1]},"
	keyframes_h.puts "\t\t\t\t\t\t#{keyframe['rotation'][2]}"
	keyframes_h.puts "\t\t\t\t\t},"
	keyframes_h.puts "\t\t\t\t\t#{keyframe['rotation'][3]}"
	keyframes_h.puts "\t\t\t\t}"
	keyframes_h.puts "\t\t\t}"
	keyframes_h.print "\t\t}"
	keyframes_h.print "," unless i == num_keyframes - 1
	keyframes_h.puts
end

keyframes_h.puts "\t}"
keyframes_h.puts "};"
