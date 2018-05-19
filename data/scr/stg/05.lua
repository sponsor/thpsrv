module("data.scr.stg.05",package.seeall)

-- stage
Stage = {}

function Stage.new()
	local bs = require("data.scr.BaseStage")
	self = {
		id = 5,
		name = "’|—Ñ",
		thumnail = "data/scr/stg/05/t05.png",
		thumnail_w = 128,
		thumnail_h = 128,
		stage = "data/scr/stg/05/s05.png",
		stage_w = 800,
		stage_h = 925,
		bg = "data/scr/stg/05/b05.png",
		bg_w = 800,
		bg_h = 725,
		bgm = "data/bgm/stage03.ogg",
		blt = {},
		se = {},
		OnEraseStage = function(self,blt_type,chr_obj_no,obj_no,px,py,pxls)
			rnd = math.random(4,6)
			for i=0,rnd do
				if math.random(1,2) == 1 then
					effect_no = C_AddEffect(DEF_STAGE_ID,0,1008,16,1024,px,py,20)
				else
					effect_no = C_AddEffect(DEF_STAGE_ID,16,1008,32,1024,px,py,20)
				end
				if effect_no ~= -1 then
					C_SetEffectAlpha(effect_no, 128)
					size = math.random()+1
					C_SetEffectScale(effect_no, size,size)
					C_SetEffectRotation(effect_no, math.random(0,5)-3)
					fvx=math.random(0,18)-9
					C_SetEffectVector(effect_no, fvx,-math.random(0,2),-fvx*(math.random()*0.15),0.4)
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
