module("data.scr.stg.00",package.seeall)

-- stage
Stage = {}

function Stage.new()
	local bs = require("data.scr.BaseStage")
	local blt1  = {
		hit_range = 10,
		bomb_range= 100,
		add_vec_x = 0,
		add_vec_y = 20,
		atk = 100,
		tex_x = 32,
		tex_y = 600,
		tex_w = 32,
		tex_h = 32,
		se = {},
	}
	setmetatable(blt1,{ __index = bs.BaseBullet.new()})
	
	self = {
		id = 0,
		name = "îéóÌê_é–",
		thumnail = "data/scr/stg/00/t00.png",
		thumnail_w = 128,
		thumnail_h = 128,
		stage = "data/scr/stg/00/s00.png",
		stage_w = 1024,
		stage_h = 820,
		bg = "data/scr/stg/00/b00.png",
		bg_w = 1024,
		bg_h = 820,
		bgm = "data/bgm/stage00.ogg",
		blt = {blt1},
		se = {},
		OnEraseStage = function(self,blt_type,chr_obj_no,obj_no,px,py,pxls)
			rnd = math.random(4,6)
			for i=0,rnd do
				effect_no = C_AddEffect(DEF_STAGE_ID,0,1008,16,1024,px,py,20)
				if effect_no ~= -1 then
					C_SetEffectAlpha(effect_no, 128)
					size = math.random()+1
					C_SetEffectScale(effect_no, size,size)
					C_SetEffectRotation(effect_no, 10)
					C_SetEffectVector(effect_no, math.random(0,10)-5,-math.random(1,8),0,0.5)
				end
			end
		end
	}
	self.blt_type_count = #self.blt
	for i,v in pairs(self.blt) do
		for j,w in pairs(v.se) do
			table.insert(self.se, w)
		end
	end
	return setmetatable(self,{ __index = bs.BaseStage.new()})
end

