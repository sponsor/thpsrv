module("data.scr.stg.04",package.seeall)

-- stage
Stage = {}

function Stage.new()
	local bs = require("data.scr.BaseStage")
	
	self = {
		id = 4,
		name = "ƒIƒ“ƒoƒVƒ‰",
		thumnail = "data/scr/stg/04/t04.png",
		thumnail_w = 128,
		thumnail_h = 128,
		stage = "data/scr/stg/04/s04.png",
		stage_w = 800,
		stage_h = 625,
		bg = "data/scr/stg/04/b04.png",
		bg_w = 800,
		bg_h = 600,
		bgm = "data/bgm/stage01.ogg",
		blt = {},
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
