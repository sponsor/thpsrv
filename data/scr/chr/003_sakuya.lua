module("data.scr.chr.003_sakuya",package.seeall)

-- bullet
-- chara
Chara = {}

function Chara.new()
	local CharaID = 3
	local bc = require("data.scr.BaseChara")
	local blt1  = {
		id = CharaID,
		hit_range = 10,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 33,
		atk = 270,
		delay = 135,
		icon_x = 96,
		icon_y = 64,
		tex_x = 96,
		tex_y = 64,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/003_sakuya/sakuya_b10.wav",
				"data/scr/chr/003_sakuya/sakuya_b11.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			effect_no = C_AddEffect(self.id,192,96,256,160,bx,by,25)
			ang = math.random(0,3)*90+45
			if effect_no ~= -1 then
				ang = math.random(0,3)*90+45
				scale = math.random(75,125)*0.01
				C_SetEffectScale(effect_no,scale,scale)
				C_SetEffectFade(effect_no,-20)
				C_SetEffectRotate(effect_no,ang)
			end
			ang = math.random(0,60)+ang+60
			effect_no = C_AddEffect(self.id,192,96,256,160,px,py,25)
			if effect_no ~= -1 then
				scale = math.random(75,125)*0.01
				C_SetEffectScale(effect_no,scale,scale)
				C_SetEffectFade(effect_no,-20)
				C_SetEffectRotate(effect_no,ang)
			end
			for i=0,10 do
				effect_no = C_AddEffect(self.id,16,112,48,144,px,py,35)
				if effect_no ~= -1 then
					ang = math.random(0,359)
					evx = math.cos(math.rad(ang))
					evy = math.sin(math.rad(ang))
					rnd = math.random(15,30)*0.04
					C_SetEffectScale(effect_no,rnd,rnd)
					C_SetEffectFade(effect_no,-math.random(8,20))
					vs = (math.random()*8+6)
					C_SetEffectVector(effect_no, vs*evx,vs*evy,-evx*rnd*0.15,0.3)
				end
			end
		end,
		Shot = function(self,chr_obj_no,chr_id,blt_type,px,py,vx,vy,vec_angle,power,frame)
			C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,0,0)
			C_PlaySoundSE(self.se[1],0,0)
			return true
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			C_UpdateBulletAngle(blt_no,vec_angle)
			if frame % 2 then
				effect_no = C_AddEffect(scr_id,96,352,192,384,px,py,20)
				if effect_no ~= -1 then
					C_SetEffectFade(effect_no,-25)
					C_SetEffectRotate(effect_no, vec_angle)
				end
			end
		end,
	}
	setmetatable(blt1,{ __index = bc.BaseBullet.new()})
	local blt2 = {
		id = CharaID,
		hit_range = 7,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 20,
		atk = 105,
		delay = 170,
		icon_x = 128,
		icon_y = 64,
		tex_x = 96,
		tex_y = 64,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/003_sakuya/sakuya_b10.wav",
				"data/scr/chr/003_sakuya/sakuya_b11.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			ang = math.random(0,3)*90+45
			effect_no = C_AddEffect(self.id,192,96,256,160,bx,by,25)
			if effect_no ~= -1 then
				scale = math.random(75,125)*0.01
				C_SetEffectScale(effect_no,scale,scale)
				C_SetEffectFade(effect_no,-20)
				C_SetEffectRotate(effect_no,ang)
			end
			ang = math.random(0,60)+ang+60
			effect_no = C_AddEffect(self.id,192,96,256,160,bx,by,25)
			if effect_no ~= -1 then
				scale = math.random(75,125)*0.01
				C_SetEffectScale(effect_no,scale,scale)
				C_SetEffectFade(effect_no,-20)
				C_SetEffectRotate(effect_no,ang)
			end
			for i=0,6 do
				effect_no = C_AddEffect(self.id,16,112,48,144,bx,by,35)
				if effect_no ~= -1 then
					ang = math.random(0,359)
					evx = math.cos(math.rad(ang))
					evy = math.sin(math.rad(ang))
					rnd = math.random(15,30)*0.04
					C_SetEffectScale(effect_no,rnd,rnd)
					C_SetEffectFade(effect_no,-math.random(8,20))
					vs = (math.random()*8+6)
					C_SetEffectVector(effect_no, vs*evx,vs*evy,-evx*rnd*0.15,0.25)
				end
			end
		end,
		Shot = function(self,chr_obj_no,chr_id,blt_type,px,py,vx,vy,vec_angle,power,frame)
			if frame==0 then
				C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,1,1)
				C_PlaySoundSE(self.se[1],0,0)
			elseif frame == 4 then
				C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,1,1)
				C_PlaySoundSE(self.se[1],0,0)
			elseif frame == 8 then
				C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,1,1)
				C_PlaySoundSE(self.se[1],0,0)
				return true
			end
			return false
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			C_UpdateBulletAngle(blt_no,vec_angle)
			if frame % 2 then
				effect_no = C_AddEffect(scr_id,96,352,192,384,px,py,20)
				if effect_no ~= -1 then
					C_SetEffectFade(effect_no,-25)
					C_SetEffectRotate(effect_no, vec_angle)
				end
			end
		end,
	}
	setmetatable(blt2,{ __index = bc.BaseBullet.new()})
	local blt3 = {
		id = CharaID,
		hit_range = 8,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 27,
		atk = 83,
		delay = 170,
		icon_x = 160,
		icon_y = 64,
		tex_x = 96,
		tex_y = 64,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/003_sakuya/sakuya_b10.wav",
				"data/scr/chr/003_sakuya/sakuya_b11.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			ang = math.random(0,3)*90+45
			effect_no = C_AddEffect(self.id,192,96,256,160,bx,by,25)
			if effect_no ~= -1 then
				scale = math.random(75,125)*0.01
				C_SetEffectScale(effect_no,scale,scale)
				C_SetEffectFade(effect_no,-20)
				C_SetEffectRotate(effect_no,ang)
			end
			ang = math.random(0,60)+ang+60
			effect_no = C_AddEffect(self.id,192,96,256,160,bx,by,25)
			if effect_no ~= -1 then
				scale = math.random(75,125)*0.01
				C_SetEffectScale(effect_no,scale,scale)
				C_SetEffectFade(effect_no,-20)
				C_SetEffectRotate(effect_no,ang)
			end
			for i=0,6 do
				effect_no = C_AddEffect(self.id,16,112,48,144,bx,by,35)
				if effect_no ~= -1 then
					ang = math.random(0,359)
					evx = math.cos(math.rad(ang))
					evy = math.sin(math.rad(ang))
					rnd = math.random(15,30)*0.04
					C_SetEffectScale(effect_no,rnd,rnd)
					C_SetEffectFade(effect_no,-math.random(8,20))
					vs = (math.random()*8+6)
					C_SetEffectVector(effect_no, vs*evx,vs*evy,-evx*rnd*0.15,0.25)
				end
			end
		end,
		Shot = function(self,chr_obj_no,chr_id,blt_type,px,py,vx,vy,vec_angle,power,frame)
			if frame == 0 then
				avx = math.cos(math.rad(vec_angle+6))*power
				avy = math.sin(math.rad(vec_angle+6))*power
				C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,avx,avy,self.add_vec_x,self.add_vec_y,self.hit_range,1,1)
				C_PlaySoundSE(self.se[1],0,0)
			elseif frame == 1 then
				C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,1,1)
				C_PlaySoundSE(self.se[1],0,0)
			elseif frame == 2 then
				avx = math.cos(math.rad(vec_angle-6))*power
				avy = math.sin(math.rad(vec_angle-6))*power
				C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,avx,avy,self.add_vec_x,self.add_vec_y,self.hit_range,1,1)
				C_PlaySoundSE(self.se[1],0,0)
				return true
			end
			return false
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			C_UpdateBulletAngle(blt_no,vec_angle)
			if frame % 2 then
				effect_no = C_AddEffect(scr_id,96,352,192,384,px,py,20)
				if effect_no ~= -1 then
					C_SetEffectFade(effect_no,-25)
					C_SetEffectRotate(effect_no, vec_angle)
				end
			end
		end,
	}
	setmetatable(blt3,{ __index = bc.BaseBullet.new()})

	local spellcard = {
		id = CharaID,
		name = "幻象「ルナクロック」",
		exp = 300,
		exp_max = 900,
		hit_range = 14,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 0,
		atk = 0,
		delay = 240,
		icon_x = 192,
		icon_y = 64,
		tex_x = 0,
		tex_y = 96,
		tex_w = 64,
		tex_h = 64,
		se = {	"data/scr/chr/003_sakuya/sakuya_s00.wav",
				"data/scr/chr/003_sakuya/sakuya_s01.wav",
				"data/scr/chr/003_sakuya/sakuya_s02.wav"},
		Spell = function(self,chr_obj_no,chr_id,px,py,vx,vy,vec_angle,power,frame)
			C_CreateBullet(BLT_PROC_TYPE_SCR_SPELL,chr_obj_no,chr_id,DEF_BLT_TYPE_SPELL,OBJ_TYPE_SOLID,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,0,0)
			C_PlaySoundSE(self.se[1],0,0)
			return true
		end,
		OnFrame = function(self,blt_type,blt_no,frame,px,py,vx,vy,ex1,ex2)
			if ex2 == 0 and frame == (FPS/2) then
				C_UpdateBulletVector(blt_no,0,0,0,0)
				C_UpdateObjectType(blt_no,OBJ_TYPE_STAGE)
				C_UpdateBulletState(blt_no,DEF_STATE_WAIT)
			end
		end,
		OnUpdateState = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,state)
			if state==DEF_STATE_WAIT then
				C_SetBulletExtData2(blt_no,1)
				C_SetBulletExtData1(blt_no,5)
				C_SetBulletTextureIndex(blt_no, 1)
				effect_no = C_AddEffect(self.id,128,96,192,160,px,py,25)
				if effect_no ~= -1 then
					C_SetEffectScale(effect_no,2,2)
					C_SetEffectFade(effect_no,-15)
					C_SetEffectScalling(effect_no,-0.2,-0.2)
				end
				C_PlaySoundSE(self.se[2],0,0)
			end
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			C_UpdateBulletAngle(blt_no,frame*3)
		end,
		OnHitChara = function(self,scr_id,blt_type,blt_chr_no,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
			if exx ~= 1 and blt_chr_no ~= hit_obj_no then
				if C_RemoveBullet(blt_no,2) == true then -- blt_no,rm_type(0:normal/1:out/2:bomb)
					C_BombObject(scr_id,DEF_BLT_TYPE_SPELL,blt_chr_no,blt_no,hx,hy)
				end
				return 1 -- このイベント時点で弾のベクトルを変える場合は1を返す
			end
			return 0
		end,
		OnHitBullet = function(self,scr_id,blt_type,blt_chr_no,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
			if C_RemoveBullet(blt_no,2) == true then -- blt_no,rm_type(0:normal/1:out/2:bomb)
				C_BombObject(scr_id,DEF_BLT_TYPE_SPELL,blt_chr_no,blt_no,hx,hy)
			end
			return 1 -- このイベント時点で弾のベクトルを変える場合は1を返す
		end,
		OnHitStage = function(self,scr_id,blt_type,blt_chr_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
			if ex2 == 0 then
				C_UpdateBulletVector(blt_no,0,0,0,0)
				C_UpdateObjectType(blt_no,OBJ_TYPE_STAGE)
				C_UpdateBulletState(blt_no,DEF_STATE_WAIT)
				return 1
			end
			return 0 -- このイベント時点で弾のベクトルを変える場合は1を返す
		end,
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			effect_no = C_AddEffect(self.id,96,288,160,352,px,py,50)
			if effect_no ~= -1 then
				C_SetEffectFade(effect_no,-5)
				C_SetEffectScale(effect_no,1.5,1.5)
				C_SetEffectScalling(effect_no,0.02,0.02)
			end
			C_PlaySoundSE(self.se[3],0,0)
			name = C_GetUserName(blt_chr_no)
			msg = string.format("%qさんのルナクロックの効果が消えました", name)
			C_AddMsgLog(msg)
		end,
		OnTurnEnd = function(self,bly_type,blt_chr_no,blt_no,turn,ex1,ex2,act_obj_no)
			-- 自キャラの時、残ターン値を減らす
			if blt_chr_no == act_obj_no then
				if ex1 == 0 and ex2 == 1 then
					b = C_GetBulletInfo(blt_no)
					if C_RemoveBullet(blt_no,0) == true then
						C_BombObject(self.id,DEF_BLT_TYPE_SPELL,blt_chr_no,blt_no,b.ax,b.ay)
					end
				else
					C_SetBulletExtData1(blt_no, ex1-1)
				end
			else-- 自キャラ以外のディレイ値に加算
				-- chr_no,HP増減値,移動値増減値,ディレイ増減値,exp
				C_UpdateCharaStatus(act_obj_no,0,0,320,0)
			end
		end,
	}
	setmetatable(spellcard,{ __index = bc.BaseBullet.new()})
	
	self = {
		id = CharaID,
		name = "十六夜咲夜",
		tex_chr = "data/scr/chr/003_sakuya/sakuya.png",
		angle_range_min = 15,
		angle_range_max = 85,
		move = 90,
		delay = 520,
		max_hp = 850,
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
		tex_face_hurt_x = 0,
		tex_face_hurt_y = 256,
		tex_face_fine_msg = "当然の結果ですね",
		tex_face_normal_msg = "ナイフの回収が面倒で出し渋ってしまいました",
		tex_face_hurt_msg = "・・・無かったことにしたい",
		blt = {blt1, blt2,blt3},
		sc = spellcard,
		se = {	"data/se/spell00.wav"},
		OnTriggerFrame = function(self,type,px,py,angle,frame)
			if type == DEF_BLT_TYPE_SPELL then	-- スペルカード演出
				if frame == 0 then
					C_HideStage()
				elseif frame == 4 then
					C_PlaySoundSE(self.se[1],0,0)
					bgx = 544
					stgw = C_GetStageWidth()
					if (stgw/2) <= px then	-- ステージの右よりの場合は左側に表示
						bgx = 256
					end
					effect_no = C_AddBGEffect(self.id,256,0,512,360,bgx,235,50)
					if effect_no ~= -1 then
						C_SetBGEffectAlpha(effect_no,0)
						C_SetBGEffectFade(effect_no,10)
						C_SetBGEffectScale(effect_no,1.5,1.5)
						C_SetBGEffectVector(effect_no, 0,1,0,0)
					end
					effect_no = C_AddEffect(self.id,384,384,480,480,px,py,25)
					if effect_no ~= -1 then
						C_SetEffectFade(effect_no,-10)
						C_SetEffectScale(effect_no,1.6,1.6)
						C_SetEffectScalling(effect_no,-0.01,-0.01)
						C_SetEffectRotation(effect_no, 25)
					end
					e=math.random(12,20)
					for i=0,e do
						tex = math.random(1,2)
						if tex == 1 then
							effect_no = C_AddEffect(self.id,224,64,240,80,px,py,50)
						else
							effect_no = C_AddEffect(self.id,240,64,256,80,px,py,50)
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
				elseif frame >= 55 then
					return true
				elseif frame > 4 and frame < 45 then
					e=math.random(1,4)
					if e == 1 then
						fx = math.random(0,256)+bgx-128
						fy = math.random(0,450)
						texx = math.random(0,11)
						texy = 384
						if texx >= 6 then
							texx = texx-6
							texy = 448
						end
						texx = texx*64
						effect_no = C_AddBGEffect(self.id,texx,texy,texx+64,texy+64,fx,fy,10)
						if effect_no ~= -1 then
							scale = math.random(10,80)*0.1
							C_SetBGEffectFade(effect_no,-12)
							C_SetBGEffectAlpha(effect_no,math.random(80,120))
							C_SetBGEffectScale(effect_no,scale,scale)
							C_SetBGEffectVector(effect_no, math.random(0,2)-1,math.random(0,4)-2,0,0)
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

