module("data.scr.chr.000_reimu",package.seeall)

-- bullet
-- chara
Chara = {}

function Chara.new()
	local CharaID = 0
	local bc = require("data.scr.BaseChara")
	local blt1  = {
		id = CharaID,
		hit_range = 4,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 22,
		atk = 330,
		delay = 130,
		icon_x = 96,
		icon_y = 64,
		tex_x = 96,
		tex_y = 64,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/000_reimu/reimu_b10.wav",
				"data/scr/chr/000_reimu/reimu_b11.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			e=math.random(12,24)
			for i=0,e do
				effect_no = C_AddEffect(self.id,224,64,240,80,bx,by,25)
				if effect_no ~= -1 then
					ang = math.random(0,3)*90+45
					evx = math.cos(math.rad(ang))
					evy = math.sin(math.rad(ang))
					rnd = math.random()+0.5
					C_SetEffectScale(effect_no,rnd,rnd)
					C_SetEffectRotation(effect_no, 10)
					C_SetEffectFade(effect_no,-20*rnd)
					vs = (math.random()*8+8)
					C_SetEffectVector(effect_no, vs*evx,vs*evy,-evx*rnd*0.4,-evy*rnd*0.4)
				end
			end
		end,
		Shot = function(self,chr_obj_no,chr_id,blt_type,px,py,vx,vy,vec_angle,power,frame)
			C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,1,1)
			C_PlaySoundSE(self.se[1],0,0)
			return true
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			if vx > 0 then
				C_UpdateBulletAngle(blt_no,frame*30)
			else
				C_UpdateBulletAngle(blt_no,frame*30)
			end
		end,
	}
	
	setmetatable(blt1,{ __index = bc.BaseBullet.new()})
	local blt2 = {
		id = CharaID,
		hit_range = 11,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 44,
		atk = 170,
		delay = 150,
		icon_x = 128,
		icon_y = 64,
		tex_x = 128,
		tex_y = 64,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/000_reimu/reimu_b20.wav",
				"data/scr/chr/000_reimu/reimu_b21.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			effect_no = C_AddEffect(self.id,64,96,109,141,bx,by,50)
			if effect_no ~= -1 then
				C_SetEffectFade(effect_no,-5)
				C_SetEffectAnimation(effect_no,4,4,false)
			end
			e=math.random(12,24)
			for i=0,e do
				effect_no = C_AddEffect(self.id,224,64,240,80,bx,by,25)
				if effect_no ~= -1 then
					ang = math.random(0,3)*90+45
					evx = math.cos(math.rad(ang))
					evy = math.sin(math.rad(ang))
					rnd = math.random()+0.5
					C_SetEffectScale(effect_no,rnd,rnd)
					C_SetEffectRotation(effect_no, 10)
					C_SetEffectFade(effect_no,-15*rnd)
					vs = (math.random()*8+6)
					C_SetEffectVector(effect_no, vs*evx,vs*evy,-evx*rnd*0.4,-evy*rnd*0.4)
				end
			end
		end,
		Shot = function(self,chr_obj_no,chr_id,blt_type,px,py,vx,vy,vec_angle,power,frame)
			C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,0,1)
			C_PlaySoundSE(self.se[1],0,0)
			return true
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			if vx > 0 then
				C_UpdateBulletAngle(blt_no,frame*20)
			else
				C_UpdateBulletAngle(blt_no,frame*20)
			end
			if frame % 2 then
				effect_no = C_AddEffect(scr_id,128,64,160,96,px,py,25)
				if effect_no ~= -1 then
					C_SetEffectFade(effect_no,-20)
					C_SetEffectRotate(effect_no, frame*20)
					C_SetEffectRotation(effect_no, 40)
				end
			end
		end,
	}
	setmetatable(blt2,{ __index = bc.BaseBullet.new()})
	local spellcard = {
		id = CharaID,
		name = "霊符「夢想封印」",
		exp = 480,
		exp_max = 480,
		hit_range = 24,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 0,
		atk = 0,
		delay = 160,
		icon_x = 160,
		icon_y = 64,
		tex_x = 160,
		tex_y = 64,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/000_reimu/reimu_b10.wav",
				"data/scr/chr/000_reimu/reimu_b21.wav"},
		Spell = function(self,chr_obj_no,chr_id,px,py,vx,vy,vec_angle,power,frame)
			C_CreateBullet(BLT_PROC_TYPE_SCR_SPELL,chr_obj_no,chr_id,DEF_BLT_TYPE_SPELL,OBJ_TYPE_GAS,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,0,0)
			C_PlaySoundSE(self.se[1],0,0)
			return true
		end,
		OnFrame = function(self,blt_type,blt_no,frame,px,py,vx,vy,ex1,ex2)
			if frame == (FPSO/2) then
				C_UpdateBulletVector(blt_no,0,0,0,0)
				C_UpdateObjectType(blt_no,OBJ_TYPE_STAGE)
				C_UpdateBulletState(blt_no,DEF_STATE_WAIT)
			end
		end,
		OnUpdateState = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,state)
			if state==DEF_STATE_WAIT then
				C_SetBulletExtData1(blt_no, 1)
				C_SetBulletTextureIndex(blt_no, 1)
			end
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			if vx > 0 then
				C_UpdateBulletAngle(blt_no,frame*12)
			else
				C_UpdateBulletAngle(blt_no,frame*12)
			end
		end,
		OnHitChara = function(self,scr_id,blt_type,blt_chr_no,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
			return 0 -- このイベント時点で弾のベクトルを変える場合は1を返す
		end,
		OnHitBullet = function(self,scr_id,blt_type,blt_chr_no,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
			if ex1 == 0 then
				return 0
			end
			hb = C_GetBulletInfo(hit_obj_no)
			hc_no = tonumber(hb.chr_obj_no)
			ht = C_GetUserTeamNo(hc_no)
			bt = C_GetUserTeamNo(blt_chr_no)
			if tonumber(ht) == tonumber(bt) then
				return 0
			end
			btype = tonumber(hb.proc_type)
			if btype == BLT_PROC_TYPE_SCR_CHARA or btype == BLT_PROC_TYPE_SCR_SPELL then
				hatk = C_GetBulletAtkValue(hit_obj_no)
				C_DamageCharaHP(blt_chr_no,hc_no,tonumber(hatk)* -0.5)
			end
			C_RemoveBullet(hit_obj_no,0)
			if C_RemoveBullet(blt_no,0) == true then -- blt_no,rm_type(0:normal/1:out/2:bomb)
				C_BombObject(scr_id,DEF_BLT_TYPE_SPELL,blt_chr_no,blt_no,hx,hy)
			end
			return 1 -- このイベント時点で弾のベクトルを変える場合は1を返す
		end,
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			effect_no = C_AddEffect(self.id,64,96,109,141,bx,by,50)
			if effect_no ~= -1 then
				C_SetEffectFade(effect_no,-5)
				C_SetEffectAnimation(effect_no,4,4,false)
				C_SetEffectScale(effect_no,1.8,1.8)
			end
		end,
	}
	setmetatable(spellcard,{ __index = bc.BaseBullet.new()})
	self = {
		id = CharaID,
		name = "博麗霊夢",
		tex_chr = "data/scr/chr/000_reimu/reimu.png",
		angle_range_min = 15,
		angle_range_max = 105,
		move = 100,
		delay = 530,
		max_hp = 820,
		draw_w = 45,
		draw_h = 45,
		tex_chr_num = 4,
		tex_chr_x = 0,
		tex_chr_y = 0,
		tex_chr_w = 32,
		tex_chr_h = 32,
		tex_gui_face_x = 0,
		tex_gui_face_y = 64,
		tex_trg_num = 4,
		tex_trg_x = 0,
		tex_trg_y = 32,
		tex_trg_w = 32,
		tex_trg_h = 32,
		tex_face_fine_msg = "今夜は宴でも開こうかしら",
		tex_face_normal_msg = "まぁこんなもんよね",
		tex_face_hurt_msg = "これはきっと異変のせいだわ・・・",
		blt = {blt1, blt2},
		sc = spellcard,
		se = {	"data/se/spell00.wav"},
		OnTriggerFrame = function(self,type,px,py,angle,frame)
			if type == DEF_BLT_TYPE_SPELL then	-- スペルカード演出
				if frame == 0 then
					C_HideStage()
				elseif frame == 4 then
					C_PlaySoundSE(self.se[1],0,0)
					bgx = 600
					stgw = C_GetStageWidth()
					if (stgw/2) <= px then	-- ステージの右よりの場合は左側に表示
						bgx = 200
					end
					effect_no = C_AddBGEffect(self.id,308,0,512,480,bgx,320,40)
					if effect_no ~= -1 then
						C_SetBGEffectAlpha(effect_no,0)
						C_SetBGEffectFade(effect_no,10)
						C_SetBGEffectScale(effect_no,2,2)
						C_SetBGEffectVector(effect_no, 0,4,0,0)
					end
					effect_no = C_AddEffect(self.id,0,96,64,160,px,py,40)
					if effect_no ~= -1 then
						C_SetEffectFade(effect_no,-6)
						C_SetEffectScale(effect_no,2.0,2.0)
						C_SetEffectScalling(effect_no,-0.03,-0.03)
						C_SetEffectRotation(effect_no, 5)
					end
					e=math.random(12,20)
					for i=0,e do
						tex = math.random(1,3)
						if tex == 1 then
							effect_no = C_AddEffect(self.id,240,64,256,80,px,py,40)
						elseif tex == 2 then
							effect_no = C_AddEffect(self.id,224,80,240,96,px,py,40)
						elseif tex == 3 then
							effect_no = C_AddEffect(self.id,240,80,256,96,px,py,40)
						end
						if effect_no ~= -1 then
							ang = math.random(1,9)*40
							evx = math.cos(math.rad(ang))
							evy = math.sin(math.rad(ang))
							rnd = math.random()+0.5
							C_SetEffectScale(effect_no,rnd,rnd)
							C_SetEffectFade(effect_no,-10)
							vs = (math.random()*8+2)
							C_SetEffectVector(effect_no, vs*evx,vs*evy,-evx*rnd*0.1,-evy*rnd*0.1)
						end
					end
				elseif frame >= 45 then
					return true
				elseif frame > 4 and frame < 40 then
					e=math.random(1,3)
					if e == 3 then
						fx = math.random(0,240)+bgx-120
						fy = math.random(50,450)
						tex = math.random(1,3)
						if tex == 1 then
							effect_no = C_AddBGEffect(self.id,0,257,64,311,fx,fy,40-frame)
						elseif tex == 2 then
							effect_no = C_AddBGEffect(self.id,64,257,128,311,fx,fy,40-frame)
						elseif tex == 3 then
							effect_no = C_AddBGEffect(self.id,128,257,192,311,fx,fy,40-frame)
						end
						if effect_no ~= -1 then
							scale = math.random()*1.5+1.2
							C_SetBGEffectAlpha(effect_no,math.random(45,75))
							C_SetBGEffectScale(effect_no,scale,scale)
							C_SetBGEffectRotation(effect_no, math.random(6,18))
							C_SetBGEffectVector(effect_no, 0,-math.random(1,3),0,0)
						end
					end
				end
			elseif frame >= 25 then
				return true
			end
			return false
		end,
	}
	self.blt_type_count = #self.blt
	self.blt_sel_count = self.blt_type_count
	for i,v in pairs(self.blt) do
		for j,w in pairs(v.se) do
			table.insert(self.se, w)
		end
	end
	for j,w in pairs(self.sc.se) do
		table.insert(self.se, w)
	end
--	local bc = require("data.scr.BaseChara")
	return setmetatable(self,{ __index = bc.BaseChara.new()})
end

