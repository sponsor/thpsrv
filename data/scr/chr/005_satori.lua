module("data.scr.chr.005_satori",package.seeall)

-- chara
Chara = {}

function Chara.new()
	local CharaID = 5
	local bc = require("data.scr.BaseChara")
	local blt0  = {
		id = CharaID,
		hit_range = 6,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 16,
		atk = 40,
		delay = 180,
		icon_x = 96,
		icon_y = 64,
		tex_x = 192,
		tex_y = 160,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/005_satori/satori_b10.wav",
				"data/scr/chr/005_satori/satori_b11.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			effect_no = C_AddEffect(self.id,0,96,32,128,bx,by,30)
			if effect_no ~= -1 then
				C_SetEffectFade(effect_no,-10)
			end
		end,
		Shot = function(self,chr_obj_no,chr_id,blt_type,px,py,vx,vy,vec_angle,power,frame)
			C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,1,1)
			C_PlaySoundSE(self.se[1],0,0)
			return true
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			if vx > 0 then
				C_UpdateBulletAngle(blt_no,frame*10)
			else
				C_UpdateBulletAngle(blt_no,frame*10)
			end
		end,
	}
	setmetatable(blt0,{ __index = bc.BaseBullet.new()})
	local blt1  = {
		id = CharaID,
		hit_range = 6,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 16,
		atk = 40,
		delay = 180,
		icon_x = 96,
		icon_y = 64,
		tex_x = 224,
		tex_y = 160,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/005_satori/satori_b10.wav",
				"data/scr/chr/005_satori/satori_b11.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			effect_no = C_AddEffect(self.id,0,96,32,128,bx,by,30)
			if effect_no ~= -1 then
				C_SetEffectFade(effect_no,-10)
			end
		end,
		Shot = function(self,chr_obj_no,chr_id,blt_type,px,py,vx,vy,vec_angle,power,frame)
			rnd = math.random(1,2)
			yy = self.add_vec_y+((frame-2)*(2-(power/DEF_MAX_SHOT_POWER)))
			if rnd == 1 then
				C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,0,1,px,py,vx,vy,self.add_vec_x,yy,self.hit_range,1,1)
			else
				C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,3,1,px,py,vx,vy,self.add_vec_x,yy,self.hit_range,1,1)
			end
			C_PlaySoundSE(self.se[1],0,0)
			if frame == 6 then
				return true
			end
			return false
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			if vx > 0 then
				C_UpdateBulletAngle(blt_no,frame*10)
			else
				C_UpdateBulletAngle(blt_no,frame*10)
			end
		end,
	}
	setmetatable(blt1,{ __index = bc.BaseBullet.new()})

	local blt2 = {
		id = CharaID,
		hit_range = 10,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 30,
		atk = 125,
		delay = 180,
		icon_x = 128,
		icon_y = 64,
		tex_x = 128,
		tex_y = 64,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/005_satori/satori_b20.wav",
				"data/scr/chr/005_satori/satori_b21.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			effect_no = C_AddEffect(self.id,32,96,76,142,bx,by,50)
			if effect_no ~= -1 then
				C_SetEffectScale(effect_no,2,2)
				C_SetEffectFade(effect_no,-5)
				C_SetEffectAnimation(effect_no,6,4,false)
			end
			for i=0,10 do
				effect_no = C_AddEffect(self.id,224,112,240,128,bx,by,45)
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
		end,
		Shot = function(self,chr_obj_no,chr_id,blt_type,px,py,vx,vy,vec_angle,power,frame)
			if frame == 0 then
				C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,1,1)
				C_PlaySoundSE(self.se[1],0,0)
			elseif frame == 10 then
				avx = math.cos(math.rad(vec_angle+3))*power
				avy = math.sin(math.rad(vec_angle+3))*power
				blt0:Shot(chr_obj_no,self.id,0,px,py,avx,avy,vec_angle,0,0)
				avx = math.cos(math.rad(vec_angle+6))*power
				avy = math.sin(math.rad(vec_angle+6))*power
				blt0:Shot(chr_obj_no,self.id,0,px,py,avx,avy,vec_angle,0,0)
				avx = math.cos(math.rad(vec_angle-3))*power
				avy = math.sin(math.rad(vec_angle-3))*power
				blt0:Shot(chr_obj_no,self.id,0,px,py,avx,avy,vec_angle,0,0)
				avx = math.cos(math.rad(vec_angle-6))*power
				avy = math.sin(math.rad(vec_angle-6))*power
				blt0:Shot(chr_obj_no,self.id,0,px,py,avx,avy,vec_angle,0,0)
				return true
			end
			return false
		end,
	}
	setmetatable(blt2,{ __index = bc.BaseBullet.new()})
	
	local blt3 = {
		id = CharaID,
		hit_range = 8,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 38,
		atk = 230,
		delay = 135,
		icon_x = 160,
		icon_y = 64,
		tex_x = 224,
		tex_y = 128,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/005_satori/satori_b30.wav",
				"data/scr/chr/005_satori/satori_b31.wav"},
		Shot = function(self,chr_obj_no,chr_id,blt_type,px,py,vx,vy,vec_angle,power,frame)
			C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,1,1)
			C_PlaySoundSE(self.se[1],0,0)
			return true
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			if math.random(0,1) == 1 then
				ang = vec_angle+math.random(0,360) --45
				evx = -math.cos(math.rad(ang))
				evy = -math.sin(math.rad(ang))
				effect_no = C_AddEffect(scr_id,224,96,240,112,px,py,25)
				if effect_no ~= -1 then
					C_SetEffectRotation(effect_no, math.random(10,20))
					C_SetEffectFade(effect_no,-math.random(10,20))
					vs = (math.random()*2+2)
					C_SetEffectVector(effect_no, vs*evx,vs*evy,-evx*0.05,-evy*0.05+0.02)
				end
			end
			C_UpdateBulletAngle(blt_no,vec_angle)
		end,
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			for i=0,16 do
				effect_no = C_AddEffect(self.id,224,96,240,112,bx,by,45)
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
		end,
	}
	setmetatable(blt3,{ __index = bc.BaseBullet.new()})

	local spellcard = {
		id = CharaID,
		name = "想起「テリブルスーヴニール」",
		exp = 250,
		exp_max = 300,
		hit_range = 10,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 50,
		atk = 200,
		delay = 125,
		icon_x = 192,
		icon_y = 64,
		tex_x = 0,
		tex_y = 448,
		tex_w = 64,
		tex_h = 64,
		se = {	"data/scr/chr/005_satori/satori_s00.wav",
				"data/scr/chr/005_satori/satori_s01.wav"},
		Spell = function(self,chr_obj_no,chr_id,px,py,vx,vy,vec_angle,power,frame)
			chr_ex1 = C_GetCharaExtData1(chr_obj_no)
			ret = true
			if chr_ex1 == 0xFF then	-- 未コピーならコピー弾
				C_CreateBullet(BLT_PROC_TYPE_SCR_SPELL,chr_obj_no,chr_id,DEF_BLT_TYPE_SPELL,OBJ_TYPE_SOLID,px,py,vx,vy,0,self.add_vec_y,self.hit_range,0,0)
				C_PlaySoundSE(self.se[1],0,0)
				return true
			else
				-- コピー済みならコピーしたスペカ発射
				--chr_obj_no,proc_type,chr_id,blt_type,angle,power,frame
				ret = C_Shot(chr_obj_no,BLT_PROC_TYPE_SCR_SPELL,chr_ex1,DEF_BLT_TYPE_SPELL,vec_angle,power,frame)
				if ret == true then
					C_SetCharaExtData1(chr_obj_no,0xFF)
					name = C_GetUserName(chr_obj_no)
					msg = string.format("%qさんのスペルカードコピーが解除されました",name)
					C_AddMsgLog(msg)
				end
			end
			return ret
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			C_SetBulletTextureIndex(blt_no, frame % 8)
			C_UpdateBulletAngle(blt_no,vec_angle)
		end,
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			effect_no = C_AddEffect(self.id,128,288,224,384,px,py-8,40)
			if effect_no ~= -1 then
				C_SetEffectFade(effect_no,-8)
				C_SetEffectScale(effect_no,1.5,1.5)
				C_SetEffectScalling(effect_no,-0.002,-0.002)
				C_SetEffectRotation(effect_no, 5)
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
		OnHitCharaBulletBomb = function(self,hit_chr_no,blt_chr_no,blt_no,hx,hy,power)
			if hit_chr_no == blt_chr_no then	-- 自爆
				return
			end
			ex1=tonumber(C_GetBulletExtData1(blt_no))
			if ex1 < (power*100) then	-- より近距離を記憶する
				hit_scr_id = C_GetScrIDFromChrNo(hit_chr_no)	-- スクリプトID取得
				if hit_scr_id ~= -1 then
					C_SetBulletExtData1(blt_no,power*100)
					if hit_scr_id == self.id then
						C_SetCharaExtData1(blt_chr_no,0xFF)	-- 同キャラの場合は元のまま
					else
						C_SetCharaExtData1(blt_chr_no,hit_scr_id)	-- ID記憶
					end
					nameh = C_GetUserName(hit_chr_no)
					nameb = C_GetUserName(blt_chr_no)
					msg = string.format("%qさんが%qさんのスペルカードをコピーしました",nameb,nameh)
					C_AddMsgLog(msg)
					C_UpdateCharaStatus(blt_chr_no,0,0,0,self.exp_max)	-- キャラObjNo,hp,mv,delay,exp
				end
			end
			return
		end,
	}
	setmetatable(spellcard,{ __index = bc.BaseBullet.new()})
	
	self = {
		id = CharaID,
		name = "古明地さとり",
		tex_chr = "data/scr/chr/005_satori/satori.png",
		angle_range_min = 35,
		angle_range_max = 60,
		move = 90,
		delay = 530,
		max_hp = 780,
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
		tex_face_fine_msg = "真似た技だって、使い方次第で本物を越えられるのよ。",
		tex_face_normal_msg = "たまには体を動かすのも、悪くないものね。",
		tex_face_hurt_msg = "心を読めても、打開策がなければ意味がないわね。",
		blt = {blt1, blt2, blt3,blt0},
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
					bgx = 650
					stgw = C_GetStageWidth()
					if (stgw/2) <= px then	-- ステージの右よりの場合は左側に表示
						bgx = 150
					end
					effect_no = C_AddBGEffect(self.id,256,0,512,340,bgx,320,55)
					if effect_no ~= -1 then
						C_SetBGEffectAlpha(effect_no,0)
						C_SetBGEffectFade(effect_no,5)
						C_SetBGEffectScale(effect_no,1.8,1.8)
						C_SetBGEffectScalling(effect_no,-0.005,-0.005)
					end
					effect_no = C_AddEffect(self.id,128,288,224,384,px,py-8,45)
					if effect_no ~= -1 then
						C_SetEffectFade(effect_no,-6)
						C_SetEffectScale(effect_no,2.0,2.0)
						C_SetEffectScalling(effect_no,-0.03,-0.03)
						C_SetEffectRotation(effect_no, 5)
					end
					for i=0,16 do
						effect_no = C_AddEffect(self.id,240,96,256,112,px,py,45)
						if effect_no ~= -1 then
							ang = math.random(0,359)
							evx = math.cos(math.rad(ang))
							evy = math.sin(math.rad(ang))
							rnd = math.random(15,30)*0.05
							C_SetEffectScale(effect_no,rnd,rnd)
							C_SetEffectFade(effect_no,-math.random(7,16))
							vs = (math.random()*8+4)
							C_SetEffectVector(effect_no, vs*evx,vs*evy,-evx*rnd*0.15,-evy*rnd*0.15)
						end
					end
				elseif frame >= 60 then
					C_ShowStage()
					return true
				elseif frame > 4 and frame < 50 then
					e=math.random(3)
					if e == 1 then
						bgx = 650
						stgw = C_GetStageWidth()
						if (stgw/2) <= px then	-- ステージの右よりの場合は左側に表示
							bgx = 150
						end
						rnd = math.random(12345)
						fx = (rnd%320)+bgx-160
						fy = (rnd%400)+40
						tex = rnd % 3
						if tex == 0 then
							effect_no = C_AddBGEffect(self.id,0,384,64,448,fx,fy,50-frame)
						elseif tex == 1 then
							effect_no = C_AddBGEffect(self.id,64,384,128,448,fx,fy,50-frame)
						elseif tex == 2 then
							effect_no = C_AddBGEffect(self.id,128,384,192,448,fx,fy,50-frame)
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
			elseif state==DEF_STATE_DEAD then
				if frame<=1 then
					if angle <= 90 or angle >= 270 then			-- 左を向いている場合
						C_SetCharaTexture(chr_no,64,64,96,96)
					else
						C_SetCharaTexture(chr_no,32,64,64,96)
					end
				end
			end
		end
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
--	local bc = require("data.scr.BaseChara")
	return setmetatable( self , { __index = bc.BaseChara.new() })
end

