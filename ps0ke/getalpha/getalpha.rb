#
# Calculate the alpha value of a color based on an composited color
# https://en.wikipedia.org/wiki/Alpha_compositing#Alpha_blending
#
# C : composite color
# F : foreground color
# B : background color
#
# C = F*α + B*(1-α)
# => α = (A - B) / (F - B)
#

if ARGV.length != 3
	$stderr.puts "Calculate the alpha value of a color based on an composited color\nusage: #{$PROGRAM_NAME} <forground hex> <background hex> <composite hex>"
	exit 1
end

RGB = [:r, :g, :b]

colors = Hash.new
colors[:foreground], colors[:background], colors[:composite] = ARGV

colors.each do |k, color|
	rgb = Hash.new
	rgb[:r], rgb[:g], rgb[:b] = color.match(/#?([0-9a-f]{2})([0-9a-f]{2})([0-9a-f]{2})/i).captures.map { |channel| channel.to_s.hex/255.0 }
	colors[k] = rgb
end

actual_background = Hash.new
RGB.each do |channel|
	actual_background[channel] = colors[:composite][channel] - colors[:background][channel]
end

foreground_background = Hash.new
RGB.each do |channel|
	foreground_background[channel] = colors[:foreground][channel] - colors[:background][channel]
end

alpha = Hash.new
RGB.each do |channel|
	alpha[channel] = (actual_background[channel] / foreground_background[channel])
end


if alpha[:r] != alpha[:g] or alpha[:r] != alpha[:b] or alpha[:g] != alpha[:b]
	$stderr.puts "alpha values do not match, this cannot be a composite color"
	exit 2
end

# i'm so going to hell for this
α = alpha[:r]

puts "float: #{α}" 
puts "hex:    " + (α * 255).to_i.to_s(16)
