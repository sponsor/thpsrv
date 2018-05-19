module("data.scr.stg.06",package.seeall)

-- stage
Stage = {}

function Stage.new()
	local bs = require("data.scr.BaseStage")
	self = {
		id = 6,
		name = "‚t‚e‚n",
		thumnail = "data/scr/stg/06/t06.png",
		thumnail_w = 128,
		thumnail_h = 128,
		stage = "data/scr/stg/06/s06.png",
		stage_w = 1024,
		stage_h = 750,
		bg = "data/scr/stg/06/b06.png",
		bg_w = 900,
		bg_h = 700,
		bgm = "data/bgm/stage00.ogg",
		blt = {blt1},
		se = {},
		OnEraseStage = function(self,blt_type,chr_obj_no,obj_no,px,py,pxls)
			b=math.floor(math.sqrt(pxls)/10)
			for i=0,b do
				rx=(math.random()-0.5)*b*12+px
				ry=(math.random()-0.5)*b*12+py
				effect_no = C_AddEffect(DEF_STAGE_ID,0,992,32,1024,rx,ry,25)
				if effect_no ~= -1 then
					s=(math.random()+0.25)*2
					C_SetEffectScale(effect_no,s,s)
					C_SetEffectAnimation(effect_no,2,12,false)
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
