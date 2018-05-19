module("data.scr.chr.012_utsuho",package.seeall)

-- 東方流星群キャラ追加データ「霊烏路空」
-- author:ごらんのすぽんさー
--
-- 【追加データ】
-- スクリプト：012_utsuho.lua
-- 画像：utsuho.png
-- 音声：utsuho_b00.wav,utsuho_b01.wav,utsuho_b10.wav,utsuho_b11.wav,utsuho_b20.wav,utsuho_b21.wav,utsuho_b22.wav,utsuho_b40.wav,utsuho_b41.wav,utsuho_s00.wav,utsuho_s01.wav,tsuho_s02.wav
--
-- 【謝辞・ライセンス】
-- ドット絵は【7B】様の作成した素材を基に作成しました。
-- [点睛集積]http://do-t.cool.ne.jp/dots/
--
-- 表情の絵はハーブ様に描いていただきました。
--
-- 効果音はマッチメイカァズ様の音声を使用させていただきました。
--
-- 【再配布について】
-- このデータのスクリプト及び画像は再配布不可とします。

-- chara
Chara = {}

function Chara.new()
	local CharaID = 12
	local bc = require("data.scr.BaseChara")
	local blt1  = {
		id = CharaID,
		hit_range = 4,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 47,
		atk = 130,
		delay = 190,
		icon_x = 96,
		icon_y = 32,
		tex_x = 96,
		tex_y = 32,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/012_utsuho/utsuho_b10.wav",
				"data/scr/chr/012_utsuho/utsuho_b11.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			effect_no = C_AddEffect(self.id,0,192,64,256,bx,by-64,24)
			if effect_no ~= -1 then
				C_SetEffectScale(effect_no,2,2)
				C_SetEffectAnimation(effect_no,3,8,false)
			end
			for i=0,30 do
				effect_no = C_AddEffect(self.id,0,96,32,128,bx,by,25)
				if effect_no ~= -1 then
					ang = math.random(0,359)
					evx = math.cos(math.rad(ang))*12
					evy = math.sin(math.rad(ang))*12
					rnd = math.random(20,30)*0.0015
					C_SetEffectAlpha(effect_no,160)
					C_SetEffectFade(effect_no,-math.random(8,16))
					C_SetEffectVector(effect_no, evx,evy,-evx*rnd,-evy*rnd)
				end
			end
		end,
		Shot = function(self,chr_obj_no,chr_id,blt_type,px,py,vx,vy,vec_angle,power,frame)
			C_PlaySoundSE(self.se[1],0,0)
			C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,OBJ_TYPE_SOLID,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,0,0)
			return true
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			ang = vec_angle+math.random(0,180)-90
			evx = math.cos(math.rad(ang))
			evy = math.sin(math.rad(ang))
			effect_no = C_AddEffect(scr_id,96,32,128,64,px,py,15)
			if effect_no ~= -1 then
				C_SetEffectScale(effect_no,0.5,0.5)
				C_SetEffectFade(effect_no,-math.random(15,25))
				vs = (math.random()*2+2)
				C_SetEffectVector(effect_no, vs*evx,vs*evy,-evx*0.05,-evy*0.05+0.02)
			end
			if vx > 0 then
				C_UpdateBulletAngle(blt_no,frame*30)
			else
				C_UpdateBulletAngle(blt_no,frame*30)
			end
		end,
	}
	setmetatable(blt1,{ __index = bc.BaseBullet.new()})
	
	local blt0 = {
		id = CharaID,
		hit_range = 5,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 18,
		atk = 36,
		delay = 190,
		icon_x = 192,
		icon_y = 64,
		tex_x = 192,
		tex_y = 64,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/012_utsuho/utsuho_b00.wav",
				"data/scr/chr/012_utsuho/utsuho_b01.wav"},
		Shot = function(self,chr_obj_no,chr_id,blt_type,px,py,vx,vy,vec_angle,power,frame)
			for i=0,5 do
				ang = i*60
				evx = math.cos(math.rad(ang))
				evy = math.sin(math.rad(ang))
				blt_no = C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px+evx*10,py+evy*10,evx*80+vx,evy*80+vy,self.add_vec_x,self.add_vec_y,self.hit_range,0,0)
			end
			return true
		end,
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			effect_no = C_AddEffect(self.id,0,64,32,96,bx,by,18)
			if effect_no ~= -1 then
				C_SetEffectAnimation(effect_no,3,6,false)
			end
		end,
		OnFrame = function(self,blt_type,blt_no,frame,px,py,vx,vy,ex1,ex2)
			if ex1==1 and frame == FPS*2 then
				C_RemoveBullet(blt_no,0)
			end
		end,
	}
	setmetatable(blt0,{ __index = bc.BaseBullet.new()})

	local blt2 = {
		id = CharaID,
		hit_range = 8,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 28,
		atk = 310,
		delay = 190,
		icon_x = 128,
		icon_y = 32,
		tex_x = 128,
		tex_y = 32,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/012_utsuho/utsuho_b20.wav",
				"data/scr/chr/012_utsuho/utsuho_b21.wav",
				"data/scr/chr/012_utsuho/utsuho_b22.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			effect_no = C_AddEffect(self.id,0,64,32,96,bx,by,18)
			if effect_no ~= -1 then
				C_SetEffectScale(effect_no,2,2)
				C_SetEffectAnimation(effect_no,3,6,false)
			end
			if ex2 == 0 then
				for i=0,12 do
					effect_no = C_AddEffect(self.id,96,32,128,64,bx,by,30)
					if effect_no ~= -1 then
						ang = math.random(0,359)
						evx = math.cos(math.rad(ang))
						evy = math.sin(math.rad(ang))
						rnd = math.random(15,30)*0.05
						C_SetEffectScale(effect_no,rnd,rnd)
						C_SetEffectFade(effect_no,-math.random(8,16))
						vs = (math.random()*8+4)
						C_SetEffectVector(effect_no, vs*evx,vs*evy,-evx*rnd*0.15,-evy*rnd*0.15)
					end
				end
			end
		end,
		Shot = function(self,chr_obj_no,chr_id,blt_type,px,py,vx,vy,vec_angle,power,frame)
			C_PlaySoundSE(self.se[1],0,0)
			ex = (1.0-(power*0.9)/DEF_MAX_SHOT_POWER)*FPS
			blt_no = C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,OBJ_TYPE_SOLID,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,ex,0)
			return true
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			return
		end,
		OnUpdateState = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,state)
			C_PlaySoundSE(self.se[2],0,0)
			C_SetBulletTextureIndex(blt_no, 1)
			for i=0,12 do
				effect_no = C_AddEffect(self.id,96,32,128,64,px,py,30)
				if effect_no ~= -1 then
					ang = math.random(0,359)
					evx = math.cos(math.rad(ang))
					evy = math.sin(math.rad(ang))
					rnd = math.random(15,30)*0.05
					C_SetEffectScale(effect_no,rnd,rnd)
					C_SetEffectFade(effect_no,-math.random(8,16))
					vs = (math.random()*8+4)
					C_SetEffectVector(effect_no,vs*evx,vs*evy,-evx*rnd*0.15,-evy*rnd*0.15)
				end
			end
		end,
		OnFrame = function(self,blt_type,blt_no,frame,px,py,vx,vy,ex1,ex2)
			if ex1 == frame then
				C_SetBulletExtData2(blt_no,1)
				C_UpdateBulletState(blt_no,DEF_STATE_ACTIVE) -- クライアント用にイベントを起こす
				b = C_GetBulletInfo(blt_no)
				blt0:Shot(b.chr_obj_no,self.id,4,px,py,vx,vy,b.angle,0,0)
			end
		end,
		OnHitCharaBulletBomb = function(self,hit_chr_no,blt_chr_no,blt_no,hx,hy,power)
			b = C_GetBulletInfo(blt_no)
			if b.extdata2 == 0 then
				C_DamageCharaHP(blt_chr_no,hit_chr_no,math.ceil(-self.atk*power))
			else
				C_DamageCharaHP(blt_chr_no,hit_chr_no,math.ceil(-90*power))
			end
		end,
	}
	setmetatable(blt2,{ __index = bc.BaseBullet.new()})
	
	local blt3  = {
		id = CharaID,
		hit_range = 30,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 22,
		atk = 100,
		delay = 135,
		icon_x = 160,
		icon_y = 64,
		tex_x = 96,
		tex_y = 288,
		tex_w = 32,
		tex_h = 32,
		bitmsk = {0x0000000f,0x000000f0,0x00000f00,0x0000f000,
				  0x000f0000,0x00f00000,0x0f000000,0xf0000000,},
		bitdel = {0xfffffff0,0xffffff0f,0xfffff0ff,0xffff0fff,
				  0xfff0ffff,0xff0fffff,0xf0ffffff,0x0fffffff,},
		se = {},
		OnHitStage = function(self,scr_id,blt_type,blt_chr_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
			return 0
		end,
		OnHitChara = function(self,scr_id,blt_type,blt_chr_no,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
			sft = hit_obj_no*4
			index = hit_obj_no+1
			dat = bit.band(self.bitmsk[index],ex1)
			hc = bit.rshift(dat,sft)	-- 取得
			ex1 = ex1-dat -- ビット範囲をクリア
			hc = (hc+1) % 15 -- カウントアップ
			if hc == 0 then
				C_DamageCharaHP(blt_chr_no,hit_obj_no,-25)	-- 弾作ったキャラObjNo,HP減らすキャラNo,減らす量
			end
			C_SetBulletExtData1(blt_no, ex1 + bit.lshift(hc,sft))
			return 0
		end,
		OnFrame = function(self,blt_type,blt_no,frame,px,py,vx,vy,ex1,ex2)
			if frame == FPS*2 then
				C_RemoveBullet(blt_no,0)
			end
		end,
	}
	setmetatable(blt3,{ __index = bc.BaseBullet.new()})

	local blt4 = {
		id = CharaID,
		hit_range = 2,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 31,
		atk = 360,
		delay = 135,
		icon_x = 96,
		icon_y = 64,
		tex_x = 0,
		tex_y = 96,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/012_utsuho/utsuho_b40.wav",
				"data/scr/chr/012_utsuho/utsuho_b41.wav"},
		Shot = function(self,chr_obj_no,chr_id,blt_type,px,py,vx,vy,vec_angle,power,frame)
			C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,0,0)
			C_PlaySoundSE(self.se[1],0,0)
			return true
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			if frame%2 == 0 then
				effect_no = C_AddEffect(self.id,0,64,32,96,px,py,18)
				if effect_no ~= -1 then
					ang = vec_angle+math.random(0,60)-30
					evx = -math.cos(math.rad(ang))
					evy = -math.sin(math.rad(ang))
					rnd = math.random()*0.3+0.4
					C_SetEffectScale(effect_no,rnd,rnd)
					C_SetEffectFade(effect_no,-10)
					C_SetEffectAnimation(effect_no,3,6,false)
					vs = (math.random()*2+2)
					C_SetEffectVector(effect_no, vs*evx,vs*evy,-evx*0.05,-evy*0.05+0.02)
				end
			end
		end,
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			effect_no = C_AddEffect(self.id,0,192,64,256,px,py,24)
			if effect_no ~= -1 then
				C_SetEffectAnimation(effect_no,3,8,false)
			end
			b = C_GetBulletInfo(blt_no)
			if b.frame_count >= FPS*3.2 then
				for i=0,30 do
					effect_no = C_AddEffect(self.id,96,32,128,64,bx,by-16,25)
					if effect_no ~= -1 then
						ang = math.random(0,359)
						evx = math.cos(math.rad(ang))*12
						evy = math.sin(math.rad(ang))*12
						rnd = math.random(20,30)*0.0015
						C_SetEffectFade(effect_no,-math.random(8,16))
						C_SetEffectVector(effect_no, evx,evy,-evx*rnd,-evy*rnd+0.2)
					end
				end
			end
		end,
		OnHitCharaBulletBomb = function(self,hit_chr_no,blt_chr_no,blt_no,hx,hy,power)
			b = C_GetBulletInfo(blt_no)
			if b ~= nil then
				atkpw = self.atk
				if b.frame_count < FPS*2.8 then
					atkpw = (b.frame_count/(FPS*2.8)) * atkpw
				end
				C_DamageCharaHP(blt_chr_no,hit_chr_no,math.ceil(-atkpw*power))
			end
		end,
	}
	setmetatable(blt4,{ __index = bc.BaseBullet.new()})
	
	local spellcard = {
		id = CharaID,
		name = "核熱「ﾆｭｰｸﾘｱﾌｭｰｼﾞｮﾝ」",
		exp = 150,
		exp_max = 700,
		hit_range = 12,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 12,
		atk = 400,
		delay = 200,
		icon_x = 192,
		icon_y = 32,
		tex_x = 192,
		tex_y = 32,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/012_utsuho/utsuho_s00.wav",
				"data/scr/chr/012_utsuho/utsuho_s01.wav",
				"data/scr/chr/012_utsuho/utsuho_s02.wav"},
		Spell = function(self,chr_obj_no,chr_id,px,py,vx,vy,vec_angle,power,frame)
			C_CreateBullet(BLT_PROC_TYPE_SCR_SPELL,chr_obj_no,chr_id,DEF_BLT_TYPE_SPELL,OBJ_TYPE_SOLID,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,0,0)
			C_PlaySoundSE(self.se[1],0,0)
			return true
		end,
		OnHitChara = function(self,scr_id,blt_type,blt_chr_no,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
			return 0
		end,
		OnHitStage = function(self,scr_id,blt_type,blt_chr_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
			if ex1 == 0 then
				C_UpdateBulletVector(blt_no,0,0,0,0)
				C_UpdateObjectType(blt_no,OBJ_TYPE_STAGE)
				C_UpdateBulletState(blt_no,DEF_STATE_WAIT)
				return 1
			end
			return 0 -- このイベント時点で弾のベクトルを変える場合は1を返す
		end,
		OnUpdateState = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,state)
			if state == DEF_STATE_WAIT then
				C_SetBulletExtData1(blt_no, 1)
				C_PlaySoundSE(self.se[2],0,0)
			else
				C_PlaySoundSE(self.se[3],0,0)
				effect_no = C_AddEffect(self.id,0,352,96,448,px,py,50)
				if effect_no ~= -1 then
					C_SetEffectScale(effect_no,0.1,0.1)
					C_SetEffectScalling(effect_no,0.2,0.2)
					C_SetEffectFade(effect_no,-4)
				end
				for i=0,40 do
					effect_no = C_AddEffect(self.id,0,448,64,512,px,py,50)
					if effect_no ~= -1 then
						ang = math.random(0,359)
						rnd = math.random()*-0.1
						evx = math.cos(math.rad(ang))*7+rnd*0.1
						evy = math.sin(math.rad(ang))*7+rnd*0.1
						scale = math.random()+0.75
						C_SetEffectScale(effect_no,scale,scale)
						C_SetEffectFade(effect_no,-6)
						C_SetEffectVector(effect_no,evx,evy,0,0)
					end
				end
				C_SetBulletExtData1(blt_no, 2)
				b = C_GetBulletInfo(blt_no)
				for i = 0,7 do
					evx = math.cos(math.rad(i*45))
					evy = math.sin(math.rad(i*45))
					C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,b.chr_obj_no,self.id,3,OBJ_TYPE_STAGE,px+evx*5,py+evy*5,evx*250,evy*250,0,0,blt3.hit_range,0,0)
					effect_no = C_AddEffect(self.id,0,256,96,352,px+evx*5,py+evy*5,FPS*2)
					if effect_no ~= -1 then
						C_SetEffectFade(effect_no,-3)
						C_SetEffectVector(effect_no,evx*2.5,evy*2.5,0,0)
					end
				end
			end
		end,
		OnTurnStart = function(self,bly_type,blt_chr_no,blt_no,turn,ex1,ex2,act_obj_no)
			if act_obj_no == blt_chr_no then
				C_UpdateBulletState(blt_no,DEF_STATE_ACTIVE)
			elseif ex1 == 2 then
				C_RemoveBullet(blt_no,0)
			end
		end,
		OnFrame = function(self,blt_type,blt_no,frame,px,py,vx,vy,ex1,ex2)
			if ex1 == 2 then
				if frame <= 30 and (frame%2) == 0 then
					ang = frame*12
					evx = math.cos(math.rad(ang))*250
					evy = math.sin(math.rad(ang))*250
					b = C_GetBulletInfo(blt_no)
					C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,b.chr_obj_no,self.id,4,OBJ_TYPE_LIQUID,px,py,evx,evy,0,0,blt0.hit_range,1,0)
				elseif frame == 31 then
					C_RemoveBullet(blt_no,0)
				end
			elseif ex1 == 0 and frame == (FPS*2) then
				C_UpdateBulletVector(blt_no,0,0,0,0)
				C_UpdateObjectType(blt_no,OBJ_TYPE_STAGE)
				C_UpdateBulletState(blt_no,DEF_STATE_WAIT)
			end
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			if math.random(0,2) == 0 then
				effect_no = C_AddEffect(self.id,96,32,128,64,px,py,30)
				if effect_no ~= -1 then
					ang = math.random(0,359)
					evx = math.cos(math.rad(ang))
					evy = math.sin(math.rad(ang))
					rnd = math.random(15,30)*0.05
					C_SetEffectScale(effect_no,rnd,rnd)
					C_SetEffectFade(effect_no,-math.random(8,16))
					C_SetEffectRotation(effect_no, math.random(10,30)-15)
					vs = (math.random()*8+4)
					C_SetEffectVector(effect_no, vs*evx,vs*evy,-evx*rnd*0.15,(-evy*rnd*0.05))
				end
			end
		end,
	}
	setmetatable(spellcard,{ __index = bc.BaseBullet.new()})
	
	self = {
		id = CharaID,
		name = "霊烏路空",
		tex_chr = "data/scr/chr/012_utsuho/utsuho.png",
		angle_range_min = -5,
		angle_range_max = 75,
		move = 55,
		delay = 550,
		max_hp = 810,
		draw_w = 45,
		draw_h = 45,
		shot_h = 12,
		tex_chr_num = 4,
		tex_chr_x = 0,
		tex_chr_y = 0,
		tex_chr_w = 32,
		tex_chr_h = 32,
		tex_gui_face_x = 0,
		tex_gui_face_y = 32,
		tex_trg_num = 4,
		tex_trg_x = 256,
		tex_trg_y = 0,
		tex_trg_w = 32,
		tex_trg_h = 32,
		tex_face_fine_x = 224,
		tex_face_fine_y = 32,
		tex_face_fine_msg = "太陽の究極のエネルギーの前に恐怖するがいい！",
		tex_face_normal_x = 320,
		tex_face_normal_y = 32,
		tex_face_normal_msg = "こんなものじゃ満足できない！もっと熱く激しくと私の中の核反応が轟き叫ぶのよ！",
		tex_face_hurt_x = 416,
		tex_face_hurt_y = 32,
		tex_face_hurt_msg = "うにゅ～。　核の反応がいまいちだなぁ　八咫烏様疲れた？",
		blt = {blt1,blt2,blt4,blt3,blt0},
		sc = spellcard,
		se = {	"data/se/spell00.wav"},
		OnDeadChara = function(self,chr_no,type,prv_stt)
			if type ~= DEF_CHARA_DEAD_DROP then
				chr = C_GetCharacterFromObjNo(chr_no)
				if chr~=nil then
					if chr.angle <= 90 or chr.angle >= 270 then			-- 左を向いている場合
						C_SetCharaTexture(chr_no,64,32,96,64)
					else
						C_SetCharaTexture(chr_no,32,32,64,64)
					end
				end
			end
		end,
		OnTriggerFrame = function(self,type,px,py,angle,frame)
			if type == DEF_BLT_TYPE_SPELL then	-- スペルカード演出
				if frame == 0 then
					C_HideStage()
				elseif frame == 4 then
					C_PlaySoundSE(self.se[1],0,0)
					bgx = 400
					effect_no = C_AddBGEffect(self.id,160,256,512,512,bgx,130,50)
					if effect_no ~= -1 then
						C_SetBGEffectFadeInOut(effect_no,6)
						C_SetBGEffectScale(effect_no,2.4,2.4)
						C_SetBGEffectVector(effect_no, 0,5,0,-0.05)
					end
					effect_no = C_AddBGEffect(self.id,0,128,512,192,bgx-128,350,50)
					if effect_no ~= -1 then
						C_SetBGEffectFadeInOut(effect_no,6)
						C_SetBGEffectScale(effect_no,2.2,2.2)
						C_SetBGEffectVector(effect_no,5,0,0,0)
					end
					effect_no = C_AddBGEffect(self.id,0,128,512,192,bgx+128,50,50)
					if effect_no ~= -1 then
						C_SetBGEffectFadeInOut(effect_no,6)
						C_SetBGEffectScale(effect_no,2.2,2.2)
						C_SetBGEffectVector(effect_no,-5,0,0,0)
					end
				elseif frame >= 50 then
					C_ShowStage()
					return true
				end
			elseif frame >= 25 then
				return true
			end
			return false
		end,
	}
	self.blt_type_count = #self.blt
	self.blt_sel_count = 3
	for i,v in pairs(self.blt) do
		for j,w in pairs(v.se) do
			table.insert(self.se, w)
		end
	end
	for j,w in pairs(self.sc.se) do
		table.insert(self.se, w)
	end
	return setmetatable( self , { __index = bc.BaseChara.new() })
end

