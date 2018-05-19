module("data.scr.chr.009_cirno",package.seeall)

-- 東方流星群キャラ追加データ「チルノ」
-- author:ごらんのすぽんさー
--
-- 【謝辞・ライセンス】
-- カットイン、フェースは難ISM様に描いていただきました。
-- http://muzuism.blog90.fc2.com/
--
-- ドット絵は【7B】様の作成した素材を基に作成しました。
-- [点睛集積]http://do-t.cool.ne.jp/dots/
--
-- 弾１ 小さい弾をいっぱい発射(少しばらける)
-- 弾２ 当たると2ターン移動不可になる(状態異常のターン数は更新される)
-- スペル 当たると2ターン移動不可・角度変更不可になる(状態異常のターンは加算される)
--        地面は削れない
-- 効果音はマッチメイカァズ様の音声を使用させていただきました。
--
-- 【再配布について】
-- このデータは東方流星群のゲームデータとしてのみ再配布可能とします。

-- chara
Chara = {}

function Chara.new()
	local CharaID = 9
	local bc = require("data.scr.BaseChara")
	local blt1  = {
		id = CharaID,
		hit_range = 1,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 9,
		atk = 19,
		delay = 160,
		icon_x = 96,
		icon_y = 64,
		tex_x = 96,
		tex_y = 64,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/009_cirno/cirno_b10.wav",
				"data/scr/chr/009_cirno/cirno_b11.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			effect_no = C_AddEffect(self.id,0,96,64,160,bx,by,20)
			if effect_no ~= -1 then
				ang = math.random(0,359)
				evx = math.cos(math.rad(ang))
				evy = math.sin(math.rad(ang))
				rnd = math.random()*0.5
				C_SetEffectScale(effect_no,rnd,rnd)
				C_SetEffectRotate(effect_no, math.random(0,359))
				vs = (math.random()*8+8)
				C_SetEffectVector(effect_no, vs*evx,vs*evy,-evx*rnd*0.4,-evy*rnd*0.4)
			end
		end,
		Shot = function(self,chr_obj_no,chr_id,blt_type,px,py,vx,vy,vec_angle,power,frame)
			if frame%4 == 0 then
				rx = vx+math.random(0,40)-20
				ry = vy+math.random(0,40)-20
				rax = math.random(0,5)-3
				ray = math.random(0,5)-3+self.add_vec_y
				C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,rx,ry,rax,ray,self.hit_range,1,1)
				C_PlaySoundSE(self.se[1],0,0)
			end
			if frame >= 80 then
				return true
			end
			return false
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			return
		end,
	}
	
	setmetatable(blt1,{ __index = bc.BaseBullet.new()})
	local blt2 = {
		id = CharaID,
		hit_range = 8,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 24,
		atk = 120,
		delay = 150,
		icon_x = 128,
		icon_y = 64,
		tex_x = 128,
		tex_y = 64,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/009_cirno/cirno_b20.wav",
				"data/scr/chr/009_cirno/cirno_b21.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			rnd = math.random(4,6)
			for i=0,rnd do
				tx = math.random(0,1)*32
				effect_no = C_AddEffect(self.id,192+tx,64,224+tx,96,px,py,20)
				if effect_no ~= -1 then
					C_SetEffectAlpha(effect_no, 128)
					size = math.random()+0.25
					C_SetEffectScale(effect_no, size,size)
					C_SetEffectRotation(effect_no, 10)
					C_SetEffectVector(effect_no, math.random(0,6)-3,-math.random(1,5),0,0.25)
				end
			end
		end,
		Shot = function(self,chr_obj_no,chr_id,blt_type,px,py,vx,vy,vec_angle,power,frame)
			C_PlaySoundSE(self.se[1])
			C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,1,1)
			return true
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			if math.random(0,1) == 1 then
				ang = vec_angle+math.random(0,60)-30
				evx = -math.cos(math.rad(ang))
				evy = -math.sin(math.rad(ang))
				tx = math.random(0,3)*64
				effect_no = C_AddEffect(scr_id,0+tx,96,64+tx,160,px,py,25)
				if effect_no ~= -1 then
					C_SetEffectScale(effect_no,0.2,0.2)
					C_SetEffectRotation(effect_no, math.random(10,20))
					C_SetEffectFade(effect_no,-math.random(10,20))
					vs = (math.random()*2+2)
					C_SetEffectVector(effect_no, vs*evx,vs*evy,-evx*0.05,-evy*0.05+0.02)
				end
			end
		end,
		OnHitCharaBulletBomb = function(self,hit_chr_no,blt_chr_no,blt_no,hx,hy,power)
			-- 移動不可状態付与
			C_SetCharaState(hit_chr_no, CHARA_STATE_NOMOVE, 2)
			-- ダメージ
			C_DamageCharaHP(blt_chr_no,hit_chr_no,-self.atk*power)
		end
	}
	setmetatable(blt2,{ __index = bc.BaseBullet.new()})

	local spellcard = {
		id = CharaID,
		name = "凍符「パーフェクトフリーズ」",
		exp = 250,
		exp_max = 700,
		hit_range = 10,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 50,
		atk = 130,
		delay = 120,
		icon_x = 160,
		icon_y = 64,
		tex_x = 160,
		tex_y = 64,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/009_cirno/cirno_s00.wav",
				"data/scr/chr/009_cirno/cirno_s01.wav"},
		Spell = function(self,chr_obj_no,chr_id,px,py,vx,vy,vec_angle,power,frame)
			C_CreateBullet(BLT_PROC_TYPE_SCR_SPELL,chr_obj_no,chr_id,DEF_BLT_TYPE_SPELL,OBJ_TYPE_SOLID,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,0,0)
			C_PlaySoundSE(self.se[1],0,0)
			return true
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			if math.random(0,1) == 1 then
				ang = vec_angle+math.random(0,60)-30
				evx = -math.cos(math.rad(ang))
				evy = -math.sin(math.rad(ang))
				tx = math.random(0,3)*64
				effect_no = C_AddEffect(scr_id,0+tx,96,64+tx,160,px,py,25)
				if effect_no ~= -1 then
					C_SetEffectScale(effect_no,0.3,0.3)
					C_SetEffectRotation(effect_no, math.random(10,20))
					C_SetEffectFade(effect_no,-math.random(10,20))
					vs = (math.random()*2+2)
					C_SetEffectVector(effect_no, vs*evx,vs*evy,-evx*0.05,-evy*0.05+0.02)
				end
			end
			if vx > 0 then
				C_UpdateBulletAngle(blt_no,frame*10)
			else
				C_UpdateBulletAngle(blt_no,frame*10)
			end
		end,
		OnHitChara = function(self,scr_id,blt_type,blt_chr_no,hit_chr_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
			if C_RemoveBullet(blt_no,2) == true then -- obj_no,rm_type(0:normal/1:out/2:bomb)
				C_BombObject(scr_id,blt_type,blt_chr_no,blt_no,hx,hy,0)
			end
			return 1
		end,
		OnHitStage = function(self,scr_id,blt_type,blt_chr_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
			if C_RemoveBullet(blt_no,2) == true then -- obj_no,rm_type(0:normal/1:out/2:bomb)
				C_BombObject(scr_id,blt_type,blt_chr_no,blt_no,hx,hy,0)
			end
			return 1
		end,
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			e=math.random(8,16)
			for i=0,e do
				tx = math.random(0,3)*64
				effect_no = C_AddEffect(self.id,0+tx,96,64+tx,160,bx,by,40)
				if effect_no ~= -1 then
					ang = math.random(0,359)
					evx = math.cos(math.rad(ang))
					evy = math.sin(math.rad(ang))
					rnd = math.random(15,30)*0.01
					C_SetEffectScale(effect_no,rnd,rnd)
					C_SetEffectFade(effect_no,-math.random(7,16))
					vs = (math.random()*8+4)
					C_SetEffectVector(effect_no, vs*evx,vs*evy,-evx*rnd*0.15,0.3)
				end
			end
			rnd = math.random(4,6)
			for i=0,rnd do
				tx = math.random(0,1)*32
				effect_no = C_AddEffect(self.id,192+tx,64,224+tx,96,px,py,20)
				if effect_no ~= -1 then
					C_SetEffectAlpha(effect_no, 128)
					size = math.random()+0.5
					C_SetEffectScale(effect_no, size,size)
					C_SetEffectRotation(effect_no, 10)
					C_SetEffectVector(effect_no, math.random(0,6)-3,-math.random(1,5),0,0)
				end
			end
			effect_no = C_AddEffect(self.id,96,416,192,512,px,py,45)
			if effect_no ~= -1 then
				C_SetEffectFade(effect_no,-6)
				C_SetEffectScale(effect_no, 1.5,1.5)
			end
		end,
		OnHitCharaBulletBomb = function(self,hit_chr_no,blt_chr_no,blt_no,hx,hy,power)
			nomove = C_GetCharaState(hit_chr_no,CHARA_STATE_NOMOVE)
			noangle = C_GetCharaState(hit_chr_no,CHARA_STATE_NOANGLE)
			-- 移動不可状態付与
			C_SetCharaState(hit_chr_no, CHARA_STATE_NOMOVE, nomove+2)
			-- 角度不可状態付与
			C_SetCharaState(hit_chr_no, CHARA_STATE_NOANGLE, noangle+2)
			-- ダメージ
			C_DamageCharaHP(blt_chr_no,hit_chr_no,-self.atk*power)
			return
		end,
	}
	setmetatable(spellcard,{ __index = bc.BaseBullet.new()})
	
	self = {
	id = CharaID,
		name = "チルノ",
		tex_chr = "data/scr/chr/009_cirno/cirno.png",
		angle_range_min = 19,
		angle_range_max = 99,
		move = 99,
		delay = 539,
		max_hp = 839,
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
		tex_face_fine_msg = "やっぱりあたいってばさいきょーね！",
		tex_face_normal_msg = "あたいの手にかかれば誰でもハラハラ！",
		tex_face_hurt_x = 0,
		tex_face_hurt_y = 256,
		tex_face_hurt_msg = "ぎゃー　今日は調子が悪くて油断しただけよ！",
		blt = {blt1, blt2},
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
					bgx = 550
					stgw = C_GetStageWidth()
					if (stgw/2) <= px then	-- ステージの右よりの場合は左側に表示
						bgx = 250
					end
					effect_no = C_AddBGEffect(self.id,256,0,512,301,bgx,220,45)
					if effect_no ~= -1 then
						C_SetBGEffectAlpha(effect_no,0)
						C_SetBGEffectFade(effect_no,10)
						C_SetBGEffectScale(effect_no,2.2,2.2)
						C_SetBGEffectVector(effect_no,0,2,0,0)
					end
				elseif frame == 15 then
					effect_no = C_AddEffect(self.id,0,416,96,512,px,py,40)
					if effect_no ~= -1 then
						C_SetEffectFade(effect_no,-6)
						C_SetEffectScalling(effect_no,-0.1,-0.1)
						C_SetEffectRotation(effect_no, 40)
					end
				elseif frame >= 50 then
					C_ShowStage()
					return true
				elseif frame > 4 and frame < 40 then
					e=math.random(1,4)
					if e == 1 then
						fx = math.random(0,500)+bgx-250
						effect_no = C_AddBGEffect(self.id,96,416,192,512,fx,math.random(150,430),45-frame)
						if effect_no ~= -1 then
							scale = math.random()*3+2
							C_SetBGEffectAlpha(effect_no,math.random(96,128))
							C_SetBGEffectScale(effect_no,scale,scale)
							C_SetBGEffectFadeInOut(effect_no,5)
							C_SetBGEffectRotation(effect_no, math.random(2,10)-6)
							C_SetBGEffectVector(effect_no, math.random()*2-1,math.random(1,10)*-1,0,math.random(1,4)*-0.1)
						end
					end
					e=math.random(1,2)
					if e == 1 then
						tx = math.random(0,3)*64
						effect_no = C_AddEffect(self.id,0+tx,96,64+tx,160,px,py,40)
						if effect_no ~= -1 then
							ang = math.random(0,359)
							evx = math.cos(math.rad(ang))
							evy = math.sin(math.rad(ang))
							rnd = math.random(15,30)*0.02
							C_SetEffectScale(effect_no,rnd,rnd)
							C_SetEffectFade(effect_no,-math.random(7,16))
							vs = (math.random()*8+4)
							C_SetEffectVector(effect_no, vs*evx,vs*evy,-evx*rnd*0.15,0.0)
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

