module("data.scr.chr.013_patchouli",package.seeall)

-- 東方流星群キャラ追加データ「パチュリー・ノーレッジ」
-- author:ごらんのすぽんさー
--
-- 【追加データ】
-- スクリプト：013_patchouli.lua
-- 画像：patchouli.png
-- 音声：patchouli_b10.wav,patchouli_b11.wav,patchouli_b20.wav,patchouli_b21.wav,patchouli_b30.wav,patchouli_b31.wav,patchouli_b32.wav,patchouli_b40.wav,patchouli_b41.wav,patchouli_b42.wav,patchouli_s00.wav,patchouli_s01.wav,patchouli_s02.wav
--
-- 【謝辞・ライセンス】
-- ドット絵は【7B】様の作成した素材を基に作成しました。
-- [点睛集積]http://do-t.cool.ne.jp/dots/
--
-- カットイン、表情の絵はハーブ様に描いていただきました。
--
-- 効果音はマッチメイカァズ様の音声を使用させていただきました。
--
-- 【再配布について】
-- このデータのスクリプト及び画像は再配布不可とします。

-- chara
Chara = {}

function Chara.new()
	local CharaID = 13
	local bc = require("data.scr.BaseChara")
	local blt1  = {
		id = CharaID,
		hit_range = 8,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 35,
		atk = 240,
		delay = 135,
		icon_x = 96,
		icon_y = 64,
		tex_x = 96,
		tex_y = 64,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/013_patchouli/patchouli_b10.wav",
				"data/scr/chr/013_patchouli/patchouli_b11.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			for i=0,16 do
				effect_no = C_AddEffect(self.id,128,64,160,96,bx,by,35)
				if effect_no ~= -1 then
					ang = math.random(0,359)
					evx = math.cos(math.rad(ang))
					evy = math.sin(math.rad(ang))
					rnd = math.random(20,30)*0.02
					C_SetEffectScale(effect_no,rnd,rnd)
					C_SetEffectFade(effect_no,-math.random(8,20))
					vs = (math.random()*8+6)
					C_SetEffectVector(effect_no, vs*evx,vs*evy,-evx*rnd*0.15,0.35)
				end
			end
		end,
		Shot = function(self,chr_obj_no,chr_id,blt_type,px,py,vx,vy,vec_angle,power,frame)
			C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,OBJ_TYPE_SOLID,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,0,0)
			C_PlaySoundSE(self.se[1],0,0)
			return true
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			C_UpdateBulletAngle(blt_no,vec_angle)
			effect_no = C_AddEffect(scr_id,224,192,256,224,px,py,20)
			if effect_no ~= -1 then
				C_SetEffectFade(effect_no,-25)
				C_SetEffectScalling(effect_no,-0.02,-0.02)
				C_SetEffectRotate(effect_no, vec_angle)
			end
		end,
	}
	setmetatable(blt1,{ __index = bc.BaseBullet.new()})
	
	local blt2 = {
		id = CharaID,
		hit_range = 8,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 24,
		atk = 96,
		delay = 170,
		icon_x = 192,
		icon_y = 192,
		tex_x = 256,
		tex_y = 224,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/013_patchouli/patchouli_b20.wav",
				"data/scr/chr/013_patchouli/patchouli_b21.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			for i=0,12 do
				effect_no = C_AddEffect(self.id,192,160,224,192,bx,by,30)
				if effect_no ~= -1 then
					ang = math.random(0,359)
					evx = math.cos(math.rad(ang))
					evy = math.sin(math.rad(ang))
					rnd = math.random(15,30)*0.05
					C_SetEffectScale(effect_no,rnd,rnd)
					C_SetEffectRotate(effect_no,ang)
					C_SetEffectFade(effect_no,-10)
					vs = (math.random()*10+6)
					C_SetEffectVector(effect_no, vs*evx,vs*evy,-evx*rnd*0.15,-evy*rnd*0.15)
				end
			end
		end,
		Shot = function(self,chr_obj_no,chr_id,blt_type,px,py,vx,vy,vec_angle,power,frame)
			if frame == 0 then
				C_PlaySoundSE(self.se[1],0,0)
				C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,vx,vy,0,self.add_vec_y,self.hit_range,0,0)
			elseif frame == 6 then
				C_PlaySoundSE(self.se[1],0,0)
				p = (power/DEF_MAX_SHOT_POWER)
				rad = math.rad(vec_angle+10)
				vx1 = math.cos(rad)
				vy1 = math.sin(rad)
				rad = math.rad(vec_angle+90)
				vx2 = math.cos(rad)*power*0.1
				vy2 = math.sin(rad)*power*0.1
				rad = math.rad(vec_angle-82)
				avx = math.cos(rad)*p*(10-vx1*5*p*(vy1+1))
				avy = math.sin(rad)*p*(10-vy1*5*p)
				C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,vx1*power+vx2,vy1*power+vy2,avx,avy+self.add_vec_y,self.hit_range,0,0)
			elseif frame == 12 then
				C_PlaySoundSE(self.se[1],0,0)
				p = (power/DEF_MAX_SHOT_POWER)
				rad = math.rad(vec_angle-10)
				vx1 = math.cos(rad)
				vy1 = math.sin(rad)
				rad = math.rad(vec_angle-90)
				vx2 = math.cos(rad)*power*0.1
				vy2 = math.sin(rad)*power*0.1
				rad = math.rad(vec_angle+82)
				avx = math.cos(rad)*p*(10-vx1*5*p*(vy1+1))
				avy = math.sin(rad)*p*(10-vy1*5*p)
				C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,vx1*power+vx2,vy1*power+vy2,avx,avy+self.add_vec_y,self.hit_range,0,0)
				return true
			end
			return false
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			C_SetBulletTextureIndex(blt_no, frame % 8)
			C_UpdateBulletAngle(blt_no,vec_angle)
			if frame % 2 then
				effect_no = C_AddEffect(scr_id,256,224,288,256,px,py,12)
				if effect_no ~= -1 then
					C_SetEffectFade(effect_no,-30)
					C_SetEffectScalling(effect_no,-0.025,-0.025)
					C_SetEffectRotate(effect_no, vec_angle)
				end
			end
		end,
	}
	setmetatable(blt2,{ __index = bc.BaseBullet.new()})
	
	local blt3  = {
		id = CharaID,
		hit_range = 16,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 0,
		atk = 0,
		delay = 300,
		icon_x = 128,
		icon_y = 64,
		tex_x = 256,
		tex_y = 192,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/013_patchouli/patchouli_b30.wav",
				"data/scr/chr/013_patchouli/patchouli_b31.wav",
				"data/scr/chr/013_patchouli/patchouli_b32.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[3],0,0)
			for i=0,20 do
				effect_no = C_AddEffect(self.id,128,64,160,96,bx,by,35)
				if effect_no ~= -1 then
					ang = math.random(0,359)
					evx = math.cos(math.rad(ang))
					evy = math.sin(math.rad(ang))
					rnd = math.random(20,40)*0.02
					C_SetEffectScale(effect_no,rnd,rnd)
					C_SetEffectFade(effect_no,-math.random(8,20))
					vs = (math.random()*8+6)
					C_SetEffectVector(effect_no, vs*evx,vs*evy,-evx*rnd*0.15,0.35)
				end
			end
		end,
		OnFrame = function(self,blt_type,blt_no,frame,px,py,vx,vy,ex1,ex2)
			if frame == (FPS/2) then
				C_UpdateBulletVector(blt_no,0,0,0,0)
				C_UpdateObjectType(blt_no,OBJ_TYPE_STAGE)
				C_UpdateBulletState(blt_no,DEF_STATE_WAIT)
			end
		end,
		OnUpdateState = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,state)
			if state==DEF_STATE_WAIT then
				C_SetBulletExtData1(blt_no, 1)
				C_PlaySoundSE(self.se[2],0,0)
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
			btype = tonumber(hb.proc_type)
			if btype == BLT_PROC_TYPE_SCR_CHARA or btype == BLT_PROC_TYPE_SCR_SPELL then
				hatk = C_GetBulletAtkValue(hit_obj_no)
				C_UpdateCharaStatus(hc_no,0,0,tonumber(hatk)*0.2,0)
			end
			C_RemoveBullet(hit_obj_no,0)
			if C_RemoveBullet(blt_no,0) == true then -- blt_no,rm_type(0:normal/1:out/2:bomb)
				C_BombObject(scr_id,blt_type,blt_chr_no,blt_no,hx,hy,0)
			end
			return 1 -- このイベント時点で弾のベクトルを変える場合は1を返す
		end,
		Shot = function(self,chr_obj_no,chr_id,blt_type,px,py,vx,vy,vec_angle,power,frame)
--			C_UpdateCharaStatus(chr_obj_no,0,0,0,-100)
			C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,OBJ_TYPE_GAS,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,0,0)
			C_PlaySoundSE(self.se[1],0,0)
			return true
		end,
		OnTurnStart = function(self,bly_type,blt_chr_no,blt_no,turn,ex1,ex2,act_obj_no)
			if blt_chr_no == act_obj_no then
				C_RemoveBullet(blt_no,0)
				C_PlaySoundSE(self.se[3],0,0)
			end
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			if (frame % 4) == 0 then
				C_SetBulletTextureIndex(blt_no, (frame/4)%8)
			end
		end,
	}
	setmetatable(blt3,{ __index = bc.BaseBullet.new()})

	local blt4 = {
		id = CharaID,
		hit_range = 8,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 20,
		atk = 46,
		delay = 140,
		icon_x = 160,
		icon_y = 64,
		tex_x = 224,
		tex_y = 160,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/013_patchouli/patchouli_b40.wav",
				"data/scr/chr/013_patchouli/patchouli_b41.wav",
				"data/scr/chr/013_patchouli/patchouli_b42.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[3],0,0)
			effect_no = C_AddEffect(self.id,0,96,64,160,bx,by,16)
			if effect_no ~= -1 then
				C_SetEffectAnimation(effect_no,2,8,false)
			end
		end,
		Shot = function(self,chr_obj_no,chr_id,blt_type,px,py,vx,vy,vec_angle,power,frame)
			C_PlaySoundSE(self.se[1],0,0)
			pw = (power/DEF_MAX_SHOT_POWER)
			ex = math.ceil(pw * FPS) + 15
			C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,vx,vy,0,self.add_vec_y,self.hit_range,ex,pw*100)
			return true
		end,
		OnFrame = function(self,blt_type,blt_no,frame,px,py,vx,vy,ex1,ex2)
			if ex2 == 0 then
				-- ターゲット中に画面外に出たら削除
				if py < 0 or px < 0 or px > C_GetStageWidth() then
					C_RemoveBullet(blt_no,1)
				end
			elseif frame > ex1 then
				-- 近距離キャラを探す
				lc_cnt = C_GetEntityCharacters()
				min_range_chr = nil
				min_range = 0
				min_range_ang = 0
				for i = 0, lc_cnt-1 do
					lchr = C_GetCharacterAtVector(i)
					rx = math.abs(px-lchr.ax)
					ry = math.abs(py-lchr.ay)
					if min_range_chr == nil or min_range > rx*rx + ry*ry then
						min_range = rx*rx + ry*ry
						min_range_ang = C_GetAngle(rx,ry)
						min_range_chr = lchr
					end
				end
				if min_range_chr ~= nil then
					if min_range < (15625-ex2*ex2) then -- 距離を確認
						ang = C_GetAngle(vy,vy)%360	-- 飛んでる角度
						if min_range_ang > ang-40 and min_range_ang < ang + 40 then
							ang = math.floor(math.abs((min_range_ang%360)-ang)) -- 角度の差分
							rnd = (math.random(0,ang*2)-ang)*4			-- 角度差分が多いほどずれる可能性を上げる
							C_UpdateBulletVector(blt_no,(min_range_chr.ax-px)*4+rnd,(min_range_chr.ay-py)*4,0,1)
							C_UpdateObjectType(blt_no,OBJ_TYPE_LIQUID)
						end
					elseif min_range < 25600 then
						ang = C_GetAngle(vy,vy)	-- 飛んでる角度
						d = 20-ex2/10
						if min_range_ang > ang-d and min_range_ang < ang+d then
							ang = math.floor(math.abs((min_range_ang%360)-ang)) -- 角度の差分
							rnd = (math.random(0,ang*2)-ang)*4			-- 角度差分が多いほどずれる可能性を上げる
							C_UpdateBulletVector(blt_no,(min_range_chr.ax-px)*4+rnd,(min_range_chr.ay-py)*4,0,1)
							C_UpdateObjectType(blt_no,OBJ_TYPE_LIQUID)
						end
					end
				end
			end
		end,
		OnUpdateType = function(self,blt_type,blt_no,px,py,vx,vy,ex1,ex2,type)
			C_SetBulletExtData1(blt_no,0xffffffff)
			C_SetBulletExtData2(blt_no,0)
			C_PlaySoundSE(self.se[2],0,0)
		end
	}
	setmetatable(blt4,{ __index = bc.BaseBullet.new()})
	
	local spellcard = {
		id = CharaID,
		name = "日符「ロイヤルフレア」",
		exp = 300,
		exp_max = 700,
		hit_range = 10,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 40,
		atk = 140,
		delay = 250,
		icon_x = 192,
		icon_y = 64,
		tex_x = 256,
		tex_y = 0,
		tex_w = 64,
		tex_h = 64,
		se = {	"data/scr/chr/013_patchouli/patchouli_s00.wav",
				"data/scr/chr/013_patchouli/patchouli_s01.wav",
				"data/scr/chr/013_patchouli/patchouli_s02.wav"},
		Spell = function(self,chr_obj_no,chr_id,px,py,vx,vy,vec_angle,power,frame)
			C_CreateBullet(BLT_PROC_TYPE_SCR_SPELL,chr_obj_no,chr_id,DEF_BLT_TYPE_SPELL,OBJ_TYPE_SOLID,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,0,0)
			C_PlaySoundSE(self.se[1],0,0)
			return true
		end,
		OnHitChara = function(self,scr_id,blt_type,blt_chr_no,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
			if ex2 == 0 then
				C_SetBulletExtData2(blt_no, 1)
				C_UpdateBulletVector(blt_no,0,0,0,20)
				C_BombObject(self.id,blt_type,blt_chr_no,blt_no,hx,hy)
				return 1
			else
				if C_RemoveBullet(blt_no,0) == true then
					C_BombObject(self.id,blt_type,blt_chr_no,blt_no,hx,hy)
				end
			end
			return 0
		end,
		OnHitStage = function(self,scr_id,blt_type,blt_chr_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
			if ex2 == 0 then
				C_UpdateBulletVector(blt_no,0,0,0,20)
				C_BombObject(self.id,blt_type,blt_chr_no,blt_no,hx,hy)
				return 1
			else
				if C_RemoveBullet(blt_no,0) == true then
					C_BombObject(self.id,blt_type,blt_chr_no,blt_no,hx,hy)
				end
			end
			return 0
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			C_SetBulletTextureIndex(blt_no, frame%4)
			if frame %2 == 0 then
				effect_no = C_AddEffect(self.id,256,160,288,192,px,py,16)
				if effect_no ~= -1 then
					ang = vec_angle+math.random(0,90)-45
					evx = -math.cos(math.rad(ang))
					evy = -math.sin(math.rad(ang))
					vs = (math.random()*2+2)
					C_SetEffectVector(effect_no, vs*evx,vs*evy,-evx*0.05,-evy*0.05+0.02)
					C_SetEffectAnimation(effect_no,2,8,false)
					C_SetEffectScale(effect_no,1.5,1.5)
				end
			end
			effect_no = C_AddEffect(self.id,192,160,224,192,px,py,20)
			if effect_no ~= -1 then
				ang = math.random(0,359)
				evx = math.cos(math.rad(ang))
				evy = math.sin(math.rad(ang))
				rnd = math.random(30,50)*0.04
				C_SetEffectScale(effect_no,rnd,rnd)
				C_SetEffectFade(effect_no,-math.random(15,20))
				vs = (math.random()*8+6)
				C_SetEffectVector(effect_no, vs*evx,vs*evy,-evx*rnd*0.05,-evy*rnd*0.05+0.4)
			end
			if ex2 == 1 then
				ang = math.random(0,359)
				evx = math.cos(math.rad(ang))
				evy = math.sin(math.rad(ang))
				vs = math.random()*50
				effect_no = C_AddEffect(self.id,256,160,288,192,px+evx*vs,py+evy*vs,32)
				if effect_no ~= -1 then
					rnd = math.random(10,20)*0.1
					C_SetEffectAnimation(effect_no,4,8,false)
					C_SetEffectScale(effect_no,rnd,rnd)
				end
			end
		end,
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			if ex2 == 0 then
				C_PlaySoundSE(self.se[2],0,0)
			else
				C_PlaySoundSE(self.se[3],0,0)
			end
			C_SetBulletExtData2(blt_no, 1)
			effect_no = C_AddEffect(self.id,0,384,96,480,bx,by,11)
			if effect_no ~= -1 then
				C_SetEffectScalling(effect_no,0.35,0.35)
				C_SetEffectFade(effect_no,-25)
			end
			effect_no = C_AddEffect(self.id,256,160,288,192,bx,by,32)
			if effect_no ~= -1 then
				rnd = math.random(25,30)*0.1
				C_SetEffectAnimation(effect_no,4,8,false)
				C_SetEffectScale(effect_no,rnd,rnd)
				C_SetEffectAlpha(effect_no,128)
			end
		end,
	}
	setmetatable(spellcard,{ __index = bc.BaseBullet.new()})
	
	self = {
		id = CharaID,
		name = "パチュリー・ノーレッジ",
		tex_chr = "data/scr/chr/013_patchouli/patchouli.png",
		angle_range_min = 20,
		angle_range_max = 100,
		move = 70,
		delay = 535,
		max_hp = 840,
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
		tex_face_fine_msg = "最高の結果ね。これで魔法の研究が大いに進むわ。",
		tex_face_normal_msg = "・・ふう。万物の根源へ至る道はまだまだ見えないわね。",
		tex_face_hurt_x = 0,
		tex_face_hurt_y = 256,
		tex_face_hurt_msg = "むきゅー！　どんな状況でも立ち回れる魔法の研究しないとダメかしら！",
		blt = {blt1,blt2,blt3,blt4},
		sc = spellcard,
		se = {	"data/se/spell00.wav"},
		OnTriggerFrame = function(self,type,px,py,angle,frame)
			if type == DEF_BLT_TYPE_SPELL then	-- スペルカード演出
				if frame == 0 then
					C_HideStage()
				elseif frame == 4 then
					C_PlaySoundSE(self.se[1],0,0)
					bgx = 520
					stgh = C_GetStageWidth()/2
					if stgh <= px then	-- ステージの右よりの場合は左側に表示
						bgx = 280
					end
					effect_no = C_AddBGEffect(self.id,128,450,512,1024,bgx,50,25)
					if effect_no ~= -1 then
						C_SetBGEffectFadeInOut(effect_no,10)
						C_SetBGEffectScale(effect_no,1.4,1.4)
						C_SetBGEffectVector(effect_no, 0,12,0,0)
					end
					effect_no = C_AddEffect(self.id,0,384,96,480,px,py,50)
					if effect_no ~= -1 then
						C_SetEffectFade(effect_no,-2)
						C_SetEffectScale(effect_no,3,3)
						C_SetEffectScalling(effect_no,-0.015,-0.015)
					end
				elseif frame >= 55 then
					C_ShowStage()
					return true
				elseif frame >= 5 and frame <= 45 then
					if frame == 20 then
						bgx = 520
						stgh = C_GetStageWidth()/2
						if stgh <= px then	-- ステージの右よりの場合は左側に表示
							bgx = 280
						end
						effect_no = C_AddBGEffect(self.id,128,257,512,800,bgx,40,40)
						if effect_no ~= -1 then
							C_SetBGEffectAlpha(effect_no,64)
							C_SetBGEffectFade(effect_no,10)
							C_SetBGEffectScale(effect_no,1.4,1.4)
							C_SetBGEffectVector(effect_no, 0,10,0,-0.2)
						end
					end
					if math.random(1,2) == 1 then
						vs = (math.random()*6+2)
						ang = math.random(0,359)
						rad = math.rad(ang)
						evx = math.cos(rad)
						evy = math.sin(rad)
						effect_no = C_AddEffect(self.id,192,224,256,256,px+vs*evx*24,py+vs*evy*24,20)
						if effect_no ~= -1 then
							rad = math.random()
							rnd = math.random()+0.5
							C_SetEffectScale(effect_no,rnd,rnd)
							C_SetEffectRotate(effect_no,ang)
							C_SetEffectFade(effect_no,-12)
							C_SetEffectVector(effect_no, -vs*evx,-vs*evy,vs*evx*0.005,vs*evy*0.005)
						end
					end
					if (frame % 2) == 0 then
						vs = (math.random()*4+1)
						ang = math.random(0,359)
						rad = math.rad(ang)
						evx = math.cos(rad)
						evy = math.sin(rad)
						effect_no = C_AddEffect(self.id,193,161,223,191,px+vs*evx*20,py+vs*evy*20,25)
						if effect_no ~= -1 then
							rad = math.random()
							rnd = math.random()+0.25
							C_SetEffectScale(effect_no,rnd,rnd)
							C_SetEffectRotate(effect_no,ang)
							C_SetEffectFade(effect_no,-10)
							C_SetEffectVector(effect_no, -vs*evx,-vs*evy,vs*evx*0.002,vs*evy*0.002)
						end
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
				if ((frame-1) % 5) == 0 then				-- 5frameごとに切り替え
					tex_idx = (frame-1) / 5 % self.tex_chr_num	-- 何番目のテクスチャを表示させるか計算
					tex_left = self.tex_chr_x					-- 初期位置を保持
					if angle <= 90 or angle >= 270 then			-- 左を向いている場合
						tex_idx = tex_idx + self.tex_chr_num	-- テクスチャ数分インデックスをずらす
					end
					tex_left = self.tex_chr_x + self.tex_chr_w * tex_idx	-- テクスチャ左端を計算
					-- テクスチャ設定
					C_SetCharaTexture(chr_no,tex_left+1,self.tex_chr_y+1,tex_left+self.tex_chr_w-1,self.tex_chr_y+self.tex_chr_h-1)
				end
			elseif state==DEF_STATE_TRIGGER_BULLET then
				if ((frame-1) % 5) == 0 then				-- 5frameごとに切り替え
					tex_idx = (frame-1) / 5
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
				if ((frame-1) % 5) == 0 then				-- 5frameごとに切り替え
					tex_idx = ((frame-1) / 5)
					if tex_idx >= 2 and tex_idx <= 8 then		-- インデックス値の修正
						tex_idx = 2
					elseif tex_idx > 8 then
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
			end
		end,
	}
	self.blt_type_count = #self.blt
	self.blt_sel_count = 4
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
