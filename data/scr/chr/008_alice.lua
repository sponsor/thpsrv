module("data.scr.chr.008_alice",package.seeall)

-- 東方流星群キャラ追加データ「アリス・マーガトロイド」
-- author:ごらんのすぽんさー
--
-- 【謝辞・ライセンス】
-- カットイン、フェースはえっぢ様に描いていただきました。
-- http://ettamu.jugem.jp/
--
-- ドット絵は【7B】様の作成した素材を基に作成しました。
-- [点睛集積]http://do-t.cool.ne.jp/dots/
--
-- 効果音はマッチメイカァズ様の音声を使用させていただきました。
--
-- 【再配布について】
-- このデータは東方流星群のゲームデータとしてのみ再配布可能とします。

-- chara
Chara = {}

function Chara.new()
	local CharaID = 8
	local bc = require("data.scr.BaseChara")
	local blt1  = {
		id = CharaID,
		hit_range = 5,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 40,
		atk = 205,
		delay = 140,
		icon_x = 96,
		icon_y = 64,
		tex_x = 96,
		tex_y = 64,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/008_alice/alice_b10.wav",
				"data/scr/chr/008_alice/alice_b11.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			effect_no = C_AddEffect(self.id,0,96,64,160,bx,by,30)
			if effect_no ~= -1 then
				C_SetEffectFade(effect_no,-8)
			end
			for i=0,8 do
				effect_no = C_AddEffect(self.id,64,96,96,128,bx,by,40)
				if effect_no ~= -1 then
					ang = math.random(0,359)
					evx = math.cos(math.rad(ang))
					evy = math.sin(math.rad(ang))
					rnd = math.random(15,30)*0.05
					C_SetEffectScale(effect_no,rnd,rnd)
					C_SetEffectFade(effect_no,-math.random(7,16))
					vs = (math.random()*8+4)
					C_SetEffectVector(effect_no, vs*evx,vs*evy,-evx*rnd*0.15,0.3)
				end
			end
			for i=0,8 do
				effect_no = C_AddEffect(self.id,192,128,256,192,px,py,45)
				if effect_no ~= -1 then
					rad = math.rad(rnd%360)
					evx = math.cos(rad)
					evy = math.sin(rad)
					tmp = (rnd%60+80)*0.01
					C_SetEffectScale(effect_no,tmp,tmp)
					tmp=(rnd%15+10)*0.001
					C_SetEffectScalling(effect_no, tmp, tmp)
					C_SetEffectFade(effect_no,-(rnd%6+10))
					evx=(math.random()*4-2)
					evy=(math.random()*4-2)
					eax=(math.random()*10-5)*0.01
					eay=(math.random()*10-5)*0.01
					C_SetEffectVector(effect_no, evx,evy,eax,eay)
				end
			end
		end,
		Shot = function(self,chr_obj_no,chr_id,blt_type,px,py,vx,vy,vec_angle,power,frame)
			tx = 0
			if vx<0 then
				tx=1
			end
			C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,tx,1)
			C_PlaySoundSE(self.se[1],0,0)
			return true
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			if vx > 0 then
				C_UpdateBulletAngle(blt_no,frame*40)
			else
				C_UpdateBulletAngle(blt_no,frame*40)
			end
		end,
	}
	setmetatable(blt1,{ __index = bc.BaseBullet.new()})
	
	local blt2 = {
		id = CharaID,
		hit_range = 5,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 25,
		atk = 75,
		delay = 140,
		icon_x = 128,
		icon_y = 64,
		tex_x = 224,
		tex_y = 96,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/008_alice/alice_b10.wav",
				"data/scr/chr/008_alice/alice_b11.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			effect_no = C_AddEffect(self.id,0,96,64,160,bx,by,30)
			if effect_no ~= -1 then
				C_SetEffectFade(effect_no,-8)
			end
			for i=0,8 do
				effect_no = C_AddEffect(self.id,64,96,96,128,bx,by,40)
				if effect_no ~= -1 then
					ang = math.random(0,359)
					evx = math.cos(math.rad(ang))
					evy = math.sin(math.rad(ang))
					rnd = math.random(15,30)*0.05
					C_SetEffectScale(effect_no,rnd,rnd)
					C_SetEffectFade(effect_no,-math.random(7,16))
					vs = (math.random()*8+4)
					C_SetEffectVector(effect_no, vs*evx,vs*evy,-evx*rnd*0.15,0.3)
				end
			end
			for i=0,8 do
				effect_no = C_AddEffect(self.id,192,128,256,192,px,py,45)
				if effect_no ~= -1 then
					rad = math.rad(rnd%360)
					evx = math.cos(rad)
					evy = math.sin(rad)
					tmp = (rnd%60+80)*0.01
					C_SetEffectScale(effect_no,tmp,tmp)
					tmp=(rnd%15+10)*0.001
					C_SetEffectScalling(effect_no, tmp, tmp)
					C_SetEffectFade(effect_no,-(rnd%6+10))
					evx=(math.random()*4-2)
					evy=(math.random()*4-2)
					eax=(math.random()*10-5)*0.01
					eay=(math.random()*10-5)*0.01
					C_SetEffectVector(effect_no, evx,evy,eax,eay)
				end
			end
		end,
		Shot = function(self,chr_obj_no,chr_id,blt_type,px,py,vx,vy,vec_angle,power,frame)
			tx = 0
			if vx<0 then
				tx=1
			end
			if frame == 0 then
				avx = math.cos(math.rad(vec_angle+10))*power
				avy = math.sin(math.rad(vec_angle+10))*power
				C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,avx,avy,self.add_vec_x,self.add_vec_y,self.hit_range,tx,1)
				C_PlaySoundSE(self.se[1],0,0)
			elseif frame == 1 then
				C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,tx,1)
				C_PlaySoundSE(self.se[1],0,0)
			elseif frame == 2 then
				avx = math.cos(math.rad(vec_angle-10))*power
				avy = math.sin(math.rad(vec_angle-10))*power
				C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,avx,avy,self.add_vec_x,self.add_vec_y,self.hit_range,tx,1)
				C_PlaySoundSE(self.se[1],0,0)
				return true
			end
			return false
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			if vx > 0 then
				C_UpdateBulletAngle(blt_no,frame*40)
			else
				C_UpdateBulletAngle(blt_no,frame*40)
			end
		end,
	}
	setmetatable(blt2,{ __index = bc.BaseBullet.new()})
	
	local blt3  = {
		id = CharaID,
		hit_range = 5,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 48,
		atk = 300,
		delay = 160,
		icon_x = 160,
		icon_y = 64,
		tex_x = 160,
		tex_y = 64,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/008_alice/alice_b10.wav",
				"data/scr/chr/008_alice/alice_b31.wav",
				"data/scr/chr/008_alice/alice_b32.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[3],0,0)
			effect_no = C_AddEffect(self.id,0,96,64,160,bx,by,30)
			if effect_no ~= -1 then
				C_SetEffectFade(effect_no,-8)
			end
			for i=0,16 do
				effect_no = C_AddEffect(self.id,64,96,96,128,bx,by,40)
				if effect_no ~= -1 then
					ang = math.random(0,359)
					evx = math.cos(math.rad(ang))
					evy = math.sin(math.rad(ang))
					rnd = math.random(15,30)*0.05
					C_SetEffectScale(effect_no,rnd,rnd)
					C_SetEffectFade(effect_no,-math.random(7,16))
					vs = (math.random()*8+4)
					C_SetEffectVector(effect_no, vs*evx,vs*evy,-evx*rnd*0.15,0.3)
				end
			end
			for i=0,8 do
				effect_no = C_AddEffect(self.id,192,128,256,192,px,py,45)
				if effect_no ~= -1 then
					rad = math.rad(rnd%360)
					evx = math.cos(rad)
					evy = math.sin(rad)
					tmp = (rnd%60+80)*0.01
					C_SetEffectScale(effect_no,tmp,tmp)
					tmp=(rnd%15+10)*0.001
					C_SetEffectScalling(effect_no, tmp, tmp)
					C_SetEffectFade(effect_no,-(rnd%6+10))
					evx=(math.random()*5-2)
					evy=(math.random()*5-2)
					eax=(math.random()*10-5)*0.01
					eay=(math.random()*10-5)*0.01
					C_SetEffectVector(effect_no, evx,evy,eax,eay)
				end
			end
		end,
		Shot = function(self,chr_obj_no,chr_id,blt_type,px,py,vx,vy,vec_angle,power,frame)
			C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,2049,0)
			C_PlaySoundSE(self.se[1],0,0)
			return true
		end,
		OnHitChara = function(self,scr_id,blt_type,blt_chr_no,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
			return 0
		end,
		OnHitStage = function(self,scr_id,blt_type,blt_chr_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
			if ex2 == 0 then
				if py < hy then
					C_SetBulletExtData2(blt_no, 2)
					C_UpdateBulletState(blt_no,DEF_STATE_WAIT)
				else
					C_UpdateBulletVector(blt_no,0,0,0,self.add_vec_y)
					C_SetBulletExtData2(blt_no, 1)
					C_SetBulletExtData1(blt_no, py)
				end
			elseif ex2 == 1 then
				if py == ex1 or py < hy then
					C_SetBulletExtData2(blt_no, 2)
					C_UpdateBulletState(blt_no,DEF_STATE_WAIT)
				else
					C_UpdateBulletVector(blt_no,0,0,0,self.add_vec_y)
					C_SetBulletExtData1(blt_no, py)
				end
			end
			return 1
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			if state~=DEF_STATE_WAIT then
				if vx > 0 then
					C_UpdateBulletAngle(blt_no,frame*40)
				else
					C_UpdateBulletAngle(blt_no,frame*40)
				end
			end
		end,
		OnUpdateState = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,state)
			if state==DEF_STATE_WAIT then
				C_UpdateBulletVector(blt_no,0,0,0,self.add_vec_y)
				C_UpdateObjectType(blt_no,OBJ_TYPE_LIQUID)
				C_PlaySoundSE(self.se[2],0,0)
			end
		end,
		OnTurnStart = function(self,blt_type,blt_chr_no,blt_no,turn,ex1,ex2,act_obj_no)
			if act_obj_no == blt_chr_no then
				blt = C_GetBulletInfo(blt_no)
				if C_RemoveBullet(blt_no,0) == true then
					C_BombObject(self.id,blt_type,blt_chr_no,blt_no,blt.ax,blt.ay)
				end
			end
		end,
	}
	setmetatable(blt3,{ __index = bc.BaseBullet.new()})
	
	local spellcard = {
		id = CharaID,
		name = "戦操「ドールズウォー」",
		exp = 150,
		exp_max = 550,
		hit_range = 10,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 35,
		atk = 100,
		delay = 180,
		icon_x = 192,
		icon_y = 64,
		tex_x = 160,
		tex_y = 96,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/008_alice/alice_b10.wav",
				"data/scr/chr/008_alice/alice_s01.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			ang = math.random(0,3)*90+45
			effect_no = C_AddEffect(self.id,96,96,160,160,bx,by,15)
			if effect_no ~= -1 then
				scale = math.random(75,125)*0.01
				C_SetEffectScale(effect_no,scale,scale)
				C_SetEffectFade(effect_no,-20)
				C_SetEffectRotate(effect_no,ang)
			end
			ang = math.random(0,60)+ang+60
			effect_no = C_AddEffect(self.id,96,96,160,160,bx,by,15)
			if effect_no ~= -1 then
				scale = math.random(75,125)*0.01
				C_SetEffectScale(effect_no,scale,scale)
				C_SetEffectFade(effect_no,-20)
				C_SetEffectRotate(effect_no,ang)
			end
		
		end,
		OnFrame = function(self,blt_type,blt_no,frame,px,py,vx,vy,ex1,ex2)
			if frame == 15 and ex2==0 then
				wind = C_GetWindValue()
				C_UpdateBulletState(blt_no,DEF_STATE_ACTIVE)
				C_UpdateBulletVector(blt_no,0,0,-wind,0)
				vang = C_GetAngle(vx,vy)--進行角度
				rang = (vang+180)%360
				C_SetBulletExtData1(blt_no, vang)
			elseif frame == 30 then
				b = C_GetBulletInfo(blt_no)
				r = math.rad(ex1)
				C_BombObject(self.id,blt_type,b.chr_obj_no,blt_no,px+math.cos(r)*45,py+math.sin(r)*45)
			elseif frame == 45 then
				C_RemoveBullet(blt_no,0) -- obj_no,rm_type(0:normal/1:out/2:bomb)
			end
		end,
		Spell = function(self,chr_obj_no,chr_id,px,py,vx,vy,vec_angle,power,frame)
			if frame == 0 then
				avx = math.cos(math.rad(vec_angle+30))* math.max(power,400)
				avy = math.sin(math.rad(vec_angle+30))* math.max(power,400)
				C_CreateBullet(BLT_PROC_TYPE_SCR_SPELL,chr_obj_no,chr_id,DEF_BLT_TYPE_SPELL,OBJ_TYPE_SOLID,px,py,avx,avy,self.add_vec_x,self.add_vec_y,self.hit_range,0,0)
				C_PlaySoundSE(self.se[1],0,0)
			elseif frame == 1 then
				avx = math.cos(math.rad(vec_angle))* math.max(power,400)
				avy = math.sin(math.rad(vec_angle))* math.max(power,400)
				C_CreateBullet(BLT_PROC_TYPE_SCR_SPELL,chr_obj_no,chr_id,DEF_BLT_TYPE_SPELL,OBJ_TYPE_SOLID,px,py,avx,avy,self.add_vec_x,self.add_vec_y,self.hit_range,0,0)
				C_PlaySoundSE(self.se[1],0,0)
			elseif frame == 2 then
				avx = math.cos(math.rad(vec_angle-30))* math.max(power,400)
				avy = math.sin(math.rad(vec_angle-30))* math.max(power,400)
				C_CreateBullet(BLT_PROC_TYPE_SCR_SPELL,chr_obj_no,chr_id,DEF_BLT_TYPE_SPELL,OBJ_TYPE_SOLID,px,py,avx,avy,self.add_vec_x,self.add_vec_y,self.hit_range,0,0)
				C_PlaySoundSE(self.se[1],0,0)
				return true
			end
			return false
		end,
		OnHitChara = function(self,scr_id,blt_type,blt_chr_no,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
			return 0 -- このイベント時点で弾のベクトルを変える場合は1を返す
		end,
		OnHitStage = function(self,scr_id,blt_type,blt_chr_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
			if ex2 == 0 then
				C_UpdateBulletState(blt_no,DEF_STATE_ACTIVE)
				vang = C_GetAngle(vx,vy)--進行角度
				rang = (vang+180)%360
				C_SetBulletExtData1(blt_no, vang)
				wind = C_GetWindValue()
				C_UpdateBulletVector(blt_no,0,0,-wind,0)
				return 1
			end
			return 0 -- このイベント時点で弾のベクトルを変える場合は1を返す
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			if ex2 ~= 0 then
				tx = 0
				if ex2 == 1 then
					tx = 1
				end
				C_SetBulletTextureIndex(blt_no, tx)
				C_UpdateBulletAngle(blt_no,0)
			else
				tx = 0
				if vx < 0 then
					tx = 1
				end
				C_SetBulletTextureIndex(blt_no, tx)
				C_UpdateBulletAngle(blt_no,0)
			end
		end,
		OnUpdateState = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,state)
			if state==DEF_STATE_ACTIVE then
				effect_no = C_AddEffect(self.id,160,128,192,160,px,py,30)
				if vx < 0 then
					C_SetBulletExtData2(blt_no, 1)
				else
					C_SetBulletExtData2(blt_no, 2)
				end
				if effect_no ~= -1 then
					C_SetEffectScalling(effect_no,0.1,0.1)
					C_SetEffectFade(effect_no,-9)
				end
			end
		end,
	}
	setmetatable(spellcard,{ __index = bc.BaseBullet.new()})
	
	self = {
		id = CharaID,
		name = "アリス・マーガトロイド",
		tex_chr = "data/scr/chr/008_alice/alice.png",
		angle_range_min = 15,
		angle_range_max = 95,
		move = 80,
		delay = 535,
		max_hp = 830,
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
		tex_face_fine_msg = "攻めてる手数は多いにかぎる。\n七色の人形の前に貴方はなにができる？",
		tex_face_normal_msg = "これが、もしオートで動く人形ならもっと楽なのかしら・・？\nそれとも・・。",
		tex_face_hurt_x = 0,
		tex_face_hurt_y = 256,
		tex_face_hurt_msg = "今日の人形のテストはこんなところね。\n付き合ってくれてありがとう。",
		blt = {blt1,blt2,blt3},
		sc = spellcard,
		se = {	"data/se/spell00.wav"},
		OnLoadChara = function(self,chr_no,px,py)
			C_SetCharaExtData1(chr_no, 0xFF)
		end,
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
					effect_no = C_AddBGEffect(self.id,256,0,512,512,bgx,20,25)
					if effect_no ~= -1 then
						C_SetBGEffectFadeInOut(effect_no,10)
						C_SetBGEffectScale(effect_no,2,2)
						C_SetBGEffectVector(effect_no, 0,12,0,0)
					end
				elseif frame == 15 then
					effect_no = C_AddEffect(self.id,0,416,96,512,px,py,40)
					if effect_no ~= -1 then
						C_SetEffectFade(effect_no,-6)
						C_SetEffectScalling(effect_no,-0.1,-0.1)
						C_SetEffectRotation(effect_no, 40)
					end
				elseif frame >= 60 then
					C_ShowStage()
					return true
				elseif frame == 30 then
					bgx = 780
					stgw = C_GetStageWidth()
					if (stgw/2) <= px then	-- ステージの右よりの場合は左側に表示
						bgx = 380
					end
					effect_no = C_AddBGEffect(self.id,192,192,256,320,bgx,48,30)
					if effect_no ~= -1 then
						C_SetBGEffectAlpha(effect_no,64)
						C_SetBGEffectFade(effect_no,10)
						C_SetBGEffectScale(effect_no,2,2)
						C_SetBGEffectVector(effect_no, -1,3,0,0)
					end
				elseif frame == 20 then
					bgx = 600
					stgw = C_GetStageWidth()
					if (stgw/2) <= px then	-- ステージの右よりの場合は左側に表示
						bgx = 200
					end
					effect_no = C_AddBGEffect(self.id,256,0,512,320,bgx,100,40)
					if effect_no ~= -1 then
						C_SetBGEffectAlpha(effect_no,64)
						C_SetBGEffectFade(effect_no,10)
						C_SetBGEffectScale(effect_no,2,2)
						C_SetBGEffectVector(effect_no, 0,12,0,-0.25)
					end
				end
			elseif frame >= 25 then
				return true
			end
			return false
		end,
		-- キャラ描画イベント
		OnDrawChara = function(self,chr_no,state,angle,vx,vy,frame)
			if state==DEF_STATE_ACTIVE or state==DEF_STATE_WAIT then	-- アクティブ状態
				if (frame % 5) == 0 then				-- 5frameごとに切り替え
					tex_idx = (frame / 5) % self.tex_chr_num	-- 何番目のテクスチャを表示させるか計算
					tex_left = self.tex_chr_x					-- 初期位置を保持
					if angle <= 90 or angle >= 270 then			-- 左を向いている場合
						tex_idx = tex_idx + self.tex_chr_num	-- テクスチャ数分インデックスをずらす
					end
					tex_left = self.tex_chr_x + self.tex_chr_w * tex_idx	-- テクスチャ左端を計算
					-- テクスチャ設定
					C_SetCharaTexture(chr_no,tex_left+1,self.tex_chr_y+1,tex_left+self.tex_chr_w-1,self.tex_chr_y+self.tex_chr_h-1)
				end
			elseif state==DEF_STATE_TRIGGER_BULLET then
				if (frame % 5) == 0 then				-- 5frameごとに切り替え
					tex_idx = (frame / 5)
					if tex_idx >= self.tex_trg_num then		-- インデックス値の修正
						tex_idx = self.tex_trg_num -1
					end
					tex_left = self.tex_trg_x					-- 初期位置を保持
					if angle <= 90 or angle >= 270 then			-- 左を向いている場合
						tex_idx = tex_idx + self.tex_trg_num	-- テクスチャ数分インデックスをずらす
					end
					tex_left = self.tex_trg_x + self.tex_trg_w * tex_idx	-- テクスチャ左端を計算
					-- テクスチャ設定
					C_SetCharaTexture(chr_no,tex_left+1,self.tex_trg_y+1,tex_left+self.tex_trg_w-1,self.tex_trg_y+self.tex_trg_h-1)
				end
			elseif state==DEF_STATE_TRIGGER_SPELL then
				if (frame % 5) == 0 then				-- 5frameごとに切り替え
					tex_idx = (frame / 5)
					if tex_idx >= 14 then					-- インデックス値の修正
						tex_idx = self.tex_trg_num -1
					elseif tex_idx < 9 then
						tex_idx = 0
					else
						tex_idx = tex_idx - 9
					end
					tex_left = self.tex_trg_x					-- 初期位置を保持
					if angle <= 90 or angle >= 270 then			-- 左を向いている場合
						tex_idx = tex_idx + self.tex_trg_num	-- テクスチャ数分インデックスをずらす
					end
					tex_left = self.tex_trg_x + self.tex_trg_w * tex_idx	-- テクスチャ左端を計算
					-- テクスチャ設定
					C_SetCharaTexture(chr_no,tex_left+1,self.tex_trg_y+1,tex_left+self.tex_trg_w-1,self.tex_trg_y+self.tex_trg_h-1)
				end
			end
		end
	}
	self.blt_type_count = #self.blt
	self.blt_sel_count = #self.blt
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

