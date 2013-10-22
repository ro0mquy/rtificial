-- du hast den code gefunden
-- tut mir leid, dass er so scheisse ist

local luafft = require "luafft"

local loading_time = 5

local music = nil
local source = nil

local spectrum = {}

local v0_last = {}
local v1_last = {}
local v2_last = {}
local v0_cum = 0
local v1_cum = 0
local v2_cum = 0

local plasma = nil
local julia = nil
local march = nil
local spike = nil
local menger = nil

local oxford = nil

local global_time = 0
local canvas2 = nil
local started = false

function love.load()
	local modes = love.graphics.getModes()
	table.sort(modes, function(a, b) return a.width*a.height > b.width*b.height end)
	love.graphics.setMode(modes[1].width, modes[1].height, true)
	music = love.sound.newSoundData("rush.mp3")
	source = love.audio.newSource(music)
	time = 0
	plasma = love.graphics.newPixelEffect("plasma.glsl")
	julia = love.graphics.newPixelEffect("julia.glsl")
	march = love.graphics.newPixelEffect("march.glsl")
	spike = love.graphics.newPixelEffect("spike.glsl")
	menger = love.graphics.newPixelEffect("menger.glsl")
	blur = love.graphics.newPixelEffect("blur.glsl")
	font = love.graphics.newFont("CAS_ANTN.TTF", 80)
	love.graphics.setFont(font)
	oxford = love.graphics.newImage("oxford.jpg")
	canvas = love.graphics.newCanvas()
	canvas2 = love.graphics.newCanvas()
end

function divide(list, factor)
	local list2 = {}
	for _,v in ipairs(list) do
		table.insert(list2, v / factor)
	end
	return list2
end

function analyze_music(cur_sample)
	local size = 2048
	local data = {}
	local comp_new = complex.new
	local insert = table.insert
	for i = cur_sample, cur_sample + size - 1 do
		local left = music:getSample(2 * i)
		local right = music:getSample(2 * i + 1)
		local sample = (left + right) / 2
		insert(data, comp_new(sample, 0))
	end

	local transformed = fft(data, false)
	transformed = divide(transformed, size/2)

	return transformed
end

function love.update(dt)
	global_time = global_time + dt
	spectrum = analyze_music(source:tell('samples'))
end

function update_avg(t, v)
	if #t == 10 then
		table.remove(t, 1)
	end
	table.insert(t, v)
	local avg = 0
	local factor_sum = 0
	for i, v in ipairs(t) do
		local factor = 1/ ( 11 - i)
		avg = avg + v * factor
		factor_sum = factor_sum + factor
	end
	avg = avg / factor_sum
	return avg
end

function love.draw()
	local width = love.graphics.getWidth()
	local height = love.graphics.getHeight()
	--[[local bars = 30

	for i=1,bars do
		local v = spectrum[i*3]:abs() + spectrum[i*3 + 1]:abs() + spectrum[i*3 + 2]:abs()
		love.graphics.rectangle('fill', (i-1) * width / bars, (1 - v) * height, width/bars, v * height)
	end
	--]]
	if global_time < loading_time then
		local loading_width = 0.6 * width
		local loading_height = 0.05 * height
		love.graphics.rectangle('line', 0.5 * (width - loading_width), 0.5 * (height - loading_height), loading_width, loading_height)
		love.graphics.rectangle('fill', 0.5 * (width - loading_width), 0.5 * (height - loading_height), global_time / loading_time * loading_width, loading_height)
	else
		if not started then
			love.audio.play(source)
			started = true
		end

		if source:isStopped() then
			love.graphics.setColor(0, 0,0)
			love.graphics.setPixelEffect()
			love.graphics.rectangle('fill', 0, 0, width, height)
			love.event.push('quit')
			return
		end
		local v0 = spectrum[1]:abs() + spectrum[2]:abs() + spectrum[3]:abs()
		local v1 = spectrum[15]:abs() + spectrum[16]:abs() + spectrum[17]:abs()
		local v2 = spectrum[30]:abs() + spectrum[31]:abs() + spectrum[32]:abs()

		v0_cum = v0_cum + v0
		v1_cum = v1_cum + v1
		v2_cum = v2_cum + v2

		v0_avg = update_avg(v0_last, v0)
		v1_avg = update_avg(v1_last, v1)
		v2_avg = update_avg(v2_last, v2)

		local position = source:tell('samples')
		local time = position / music:getSampleRate()

		function draw_effect(effect)
			love.graphics.setPixelEffect(effect)
			effect:send("val", {v0, v1, v2})
			effect:send("cum", {v0_cum, v1_cum, v2_cum})
			effect:send("avg", {v0_avg, v1_avg, v2_avg})
			effect:send("width", love.graphics.getWidth())
			effect:send("height", love.graphics.getHeight())
			effect:send("time", time)
			love.graphics.rectangle('fill', 0, 0, width, height)
		end

		love.graphics.setColor(0, 0, 0)
		if position < 1037917 then
			draw_effect(julia)
			love.graphics.setPixelEffect()
			if position < 305662 then
			elseif position < 448494 then
				love.graphics.printf("Code/\"Graphics\":\n_vincent", 0, height/2 - 40, width, "center")
			elseif position < 570378 then
				love.graphics.printf("Music:\nDr. B", 0, height/2 - 40, width, "center")
			elseif position < 712735 then
				love.graphics.printf("the wannabe demomaker\nis embarrassed to present", 0, height/2 - 40, width, "center")
			elseif position < 835095 then
				love.graphics.printf("a size-unlimited demo\nshamefully named", 0, height/2 - 40, width, "center")
			elseif position > 976975 then
				love.graphics.printf("HSV. Sooooo gut!", 0, height/2 - 40, width, "center")
			end
		elseif position < 1688281 then
			plasma:send("alpha", 1)
			draw_effect(plasma)
		elseif position < 2356262 then
			draw_effect(march)
		elseif position < 2829514 then
			draw_effect(julia)
			love.graphics.setPixelEffect()
			if position > 2422441 then
				if position < 2504331 then
					love.graphics.printf("Arrrrr!", 0, height/2 - 40, width, "center")
				elseif position < 2606219 then
					love.graphics.printf("invites you to", 0, height/2 - 40, width, "center")
				end
			end
		elseif position < 3328952 then
			love.graphics.setPixelEffect()
			love.graphics.setColor(40, 40, 40)
			love.graphics.setCanvas(canvas)
			love.graphics.rectangle('fill', 0, 0, width, height)
			love.graphics.setColor(255, 255, 255)
			love.graphics.draw(oxford, (width - oxford:getWidth()) / 2, (height - oxford:getHeight()) / 2)
			love.graphics.setCanvas(canvas2)
			love.graphics.setPixelEffect(blur)
			blur:send("sigma", 5. * v0_avg)
			blur:send("blurMultiplyVec", {1., 0.})
			love.graphics.draw(canvas)
			love.graphics.setCanvas()
			blur:send("sigma", 8. * v1_avg)
			blur:send("blurMultiplyVec", {0., 1.})
			love.graphics.draw(canvas2)
			love.graphics.setPixelEffect()
			love.graphics.printf("Arrrrr Stammtisch", 0,  80, width, "center")
			love.graphics.printf("21 Uhr - Kaiserstr. 57", 0, height -80 - 80, width, "center")
			plasma:send("alpha", 1. - (3328952 - position) / 400000)
			draw_effect(plasma)
		elseif position < 4631585 then
			draw_effect(spike)
		elseif position < 5271951 then
			draw_effect(julia)
		elseif position < 6594581 then
			draw_effect(menger)
		elseif position < 7721300 then
			plasma:send("alpha", 1.)
			draw_effect(plasma)
			love.graphics.setPixelEffect()
			if position > 6676947 then
				if position < 6839300 then
					love.graphics.printf("Greetings to", 0, height/2 - 40, width, "center")
				elseif position < 6883400 then
					love.graphics.printf("zckrbrt", 0, height/2 - 40, width, "center")
				elseif position < 6927500 then
					love.graphics.printf("luckyxxl", 0, height/2 - 40, width, "center")
				elseif position < 6971600 then
					love.graphics.printf("TehMillhouse", 0, height/2 - 40, width, "center")
				elseif position < 7015700 then
					love.graphics.printf("Ps0ke", 0, height/2 - 40, width, "center")
				elseif position < 7059800 then
					love.graphics.printf("ro0mquy", 0, height/2 - 40, width, "center")
				elseif position < 7103900 then
					love.graphics.printf("Strandtasche", 0, height/2 - 40, width, "center")
				elseif position < 7148000 then
					love.graphics.printf("Lokus", 0, height/2 - 40, width, "center")
				elseif position < 7192100 then
					love.graphics.printf("stroboholics", 0, height/2 - 40, width, "center")
				elseif position < 7236200 then
					love.graphics.printf("urs", 0, height/2 - 40, width, "center")
				elseif position < 7280300 then
					love.graphics.printf("Eisteekanne", 0, height/2 - 40, width, "center")
				elseif position < 7324400 then
					love.graphics.printf("derKha", 0, height/2 - 40, width, "center")
				elseif position < 7368500 then
					love.graphics.printf("Cakemix", 0, height/2 - 40, width, "center")
				elseif position < 7412600 then
					love.graphics.printf("Mona", 0, height/2 - 40, width, "center")
				elseif position < 7456700 then
					love.graphics.printf("Rudaba", 0, height/2 - 40, width, "center")
				elseif position < 7500800 then
					love.graphics.printf("#kitinfo", 0, height/2 - 40, width, "center")
				elseif position < 7544900 then
					love.graphics.printf("Oxford Cafe", 0, height/2 - 40, width, "center")
				elseif position < 7589000 then
					love.graphics.printf("Simon", 0, height/2 - 40, width, "center")
				elseif position < 7721300 then
					love.graphics.printf("...und an alle neuen Erstis und alt-Arrrrrler!", 0, height/2 - 40, width, "center")
				end
			end
		else
			draw_effect(march)
		end
	end
end


function love.keypressed(key)
	if key == 'escape' then
		love.event.push('quit')
	end
end
