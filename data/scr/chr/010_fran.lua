module("data.scr.chr.010_fran",package.seeall)

-- 東方流星群キャラ追加データ「フランドール・スカーレット」
-- author:ごらんのすぽんさー
--
-- 【追加データ】
-- スクリプト：010_fran.lua
-- 画像：fran.png
-- 音声：fran_b10.wav,fran_b11.wav,fran_b12.wav,fran_b20.wav,fran_b21.wav,fran_b22.wav,fran_s01.wav
--
-- 【謝辞・ライセンス】
-- ドット絵は【7B】様の作成した素材を基に作成しました。
-- [点睛集積]http://do-t.cool.ne.jp/dots/
--
-- 効果音はマッチメイカァズ様の音声を使用させていただきました。
--
-- 【再配布について】
-- このデータのスクリプト及び画像は再配布不可とします。

-- chara
Chara = {}

function Chara.new()
	local CharaID = 10
	local bc = require("data.scr.BaseChara")
	local blt1  = {
		id = CharaID,
		hit_range = 12,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 26,
		atk = 170,
		delay = 180,
		icon_x = 96,
		icon_y = 64,
		tex_x = 0,
		tex_y = 128,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/010_fran/fran_b10.wav",
				"data/scr/chr/010_fran/fran_b11.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			effect_no = C_AddEffect(self.id,0,466,44,512,bx,by,50)
			if effect_no ~= -1 then
				C_SetEffectScale(effect_no,2,2)
				C_SetEffectFade(effect_no,-5)
				C_SetEffectAnimation(effect_no,8,4,false)
			end
		end,
		Shot = function(self,chr_obj_no,chr_id,blt_type,px,py,vx,vy,vec_angle,power,frame)
			if frame == 0 then
				C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,OBJ_TYPE_SOLID,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,0,0)
				C_PlaySoundSE(self.se[1],0,0)
			elseif frame == 3 or frame == 6 then
				p = (math.random(95,105)*0.01)
				C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,3,OBJ_TYPE_SOLID,px,py,vx*p,vy*p,self.add_vec_x,self.add_vec_y,self.hit_range,0,0)
				C_PlaySoundSE(self.se[1],0,0)
			elseif frame == 9 or frame == 12 or frame == 15 then
				p = (math.random(95,105)*0.01)
				C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,4,OBJ_TYPE_SOLID,px,py,vx*p,vy*p,self.add_vec_x,self.add_vec_y,self.hit_range,0,0)
				C_PlaySoundSE(self.se[1],0,0)
			elseif frame > 15 then
				return true
			end
			return false
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			return
		end,
	}
	setmetatable(blt1,{ __index = bc.BaseBullet.new()})
	
	local blt3  = {
		id = CharaID,
		hit_range = 8,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 18,
		atk = 45,
		delay = 180,
		icon_x = 96,
		icon_y = 64,
		tex_x = 32,
		tex_y = 128,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/010_fran/fran_b12.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[1],0,0)
			for i=0,10 do
				effect_no = C_AddEffect(self.id,192,96,256,160,bx,by,30)
				if effect_no ~= -1 then
					ang = math.random(0,359)
					evx = math.cos(math.rad(ang))
					evy = math.sin(math.rad(ang))
					rnd = math.random(15,30)*0.04
					C_SetEffectScale(effect_no,rnd,rnd)
					C_SetEffectFade(effect_no,-math.random(8,16))
					vs = (math.random()*8+4)
					C_SetEffectVector(effect_no, vs*evx,vs*evy,-evx*rnd*0.15,0.3)
				end
			end
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			return
		end,
	}
	setmetatable(blt3,{ __index = bc.BaseBullet.new()})

	local blt4  = {
		id = CharaID,
		hit_range = 6,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 16,
		atk = 25,
		delay = 180,
		icon_x = 96,
		icon_y = 64,
		tex_x = 64,
		tex_y = 128,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/010_fran/fran_b12.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[1],0,0)
			for i=0,5 do
				effect_no = C_AddEffect(self.id,192,96,256,160,bx,by,30)
				if effect_no ~= -1 then
					ang = math.random(0,359)
					evx = math.cos(math.rad(ang))
					evy = math.sin(math.rad(ang))
					rnd = math.random(15,30)*0.03
					C_SetEffectScale(effect_no,rnd,rnd)
					C_SetEffectFade(effect_no,-math.random(8,16))
					vs = (math.random()*8+4)
					C_SetEffectVector(effect_no, vs*evx,vs*evy,-evx*rnd*0.15,0.3)
				end
			end
		end,		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			return
		end,
	}
	setmetatable(blt4,{ __index = bc.BaseBullet.new()})

	local blt0 = {
		id = CharaID,
		hit_range = 4,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 13,
		atk = 68,
		delay = 170,
		icon_x = 128,
		icon_y = 64,
		tex_x = 16,
		tex_y = 80,
		tex_w = 16,
		tex_h = 16,
		se = {	"data/scr/chr/010_fran/fran_b22.wav"},
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			return
		end,
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[1],0,0)
			for i=0,8 do
				effect_no = C_AddEffect(self.id,16,80,32,96,bx,by,25)
				if effect_no ~= -1 then
					ang = math.random(0,3)*90+45
					evx = math.cos(math.rad(ang))
					evy = math.sin(math.rad(ang))
					rnd = math.random()*0.5+0.3
					C_SetEffectScale(effect_no,rnd,rnd)
					C_SetEffectFade(effect_no,-15*rnd)
					vs = (math.random()*6+4)
					C_SetEffectVector(effect_no, vs*evx,vs*evy,-evx*rnd*0.3,-evy*rnd*0.3)
				end
			end
		end,
	}
	setmetatable(blt0,{ __index = bc.BaseBullet.new()})
	
	local blt2 = {
		id = CharaID,
		hit_range = 0,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 16,
		atk = 300,
		delay = 170,
		icon_x = 128,
		icon_y = 64,
		tex_x = 0,
		tex_y = 96,
		tex_w = 192,
		tex_h = 32,
		se = {	"data/scr/chr/010_fran/fran_b20.wav",
				"data/scr/chr/010_fran/fran_b21.wav"},
		Shot = function(self,chr_obj_no,chr_id,blt_type,px,py,vx,vy,vec_angle,power,frame)
			blt_no = C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,OBJ_TYPE_STAGE,px,py,0,0,0,0,0,vec_angle+360,power)
			if blt_no ~= -1 then
				C_UpdateBulletAngle(blt_no,vec_angle+360)
			end
			C_PlaySoundSE(self.se[1],0,0)
			return true
		end,
		OnFrame = function(self,type,blt_no,frame,px,py,vx,vy,ex1,ex2)
			if frame == 20 then
				C_PlaySoundSE(self.se[2],0,0)
			elseif frame >= 45 then
				r = math.rad(ex1)
				rx = math.cos(r)
				ry = math.sin(r)
				blt = C_GetBulletInfo(blt_no)
				for i=0,4 do
					C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,blt.chr_obj_no,self.id,2,OBJ_TYPE_SOLID,rx*(i*10+10)+px,ry*(i*10+10)+py,rx*ex2,ry*ex2,self.add_vec_x,self.add_vec_y,self.hit_range,0,0)
				end
				C_RemoveBullet(blt_no,0)
			end
		end,
		OnHitChara = function(self,scr_id,blt_type,blt_chr_no,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
			return 0
		end,
		OnHitStage = function(self,scr_id,blt_type,blt_chr_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
			return 0
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			if frame <= 20 then
				C_UpdateBulletAngle(blt_no,ex1+frame*18)
			end
		end,
	}
	setmetatable(blt2,{ __index = bc.BaseBullet.new()})
	
	local spellcard = {
		id = CharaID,
		name = "禁忌「フォーオブアカインド」",
		exp = 0,
		exp_max = 750,
		hit_range = 0,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 35,
		atk = 0,
		delay = 360,
		icon_x = 160,
		icon_y = 64,
		tex_x = 192,
		tex_y = 256,
		tex_w = 64,
		tex_h = 64,
		se = {	"data/scr/chr/010_fran/fran_s01.wav",
				"data/se/sgi_Stealth.wav"},
		OnFrame = function(self,blt_type,blt_no,frame,px,py,vx,vy,ex1,ex2)
			if frame == ex2*(FPSO*1.5) then	-- 削除する順番
				C_UpdateBulletState(blt_no,DEF_STATE_WAIT)
			end
		end,
		Spell = function(self,chr_obj_no,chr_id,px,py,vx,vy,vec_angle,power,frame)
			rnd = math.random(1,4)
			C_SetCharaState(chr_obj_no, CHARA_STATE_STEALTH,2) -- ステルス
			for i=1,4 do
				pos = C_GetRandomStagePos()
				if rnd == i then
					C_CreateBullet(BLT_PROC_TYPE_SCR_SPELL,chr_obj_no,chr_id,DEF_BLT_TYPE_SPELL,OBJ_TYPE_STAGE,pos.x,pos.y-8,0,0,0,0,0,chr_obj_no,i)
				else
					C_CreateBullet(BLT_PROC_TYPE_SCR_SPELL,chr_obj_no,chr_id,DEF_BLT_TYPE_SPELL,OBJ_TYPE_STAGE,pos.x,pos.y-8,0,0,0,0,0,0xff,i)
				end
			end
			C_PlaySoundSE(self.se[1],0,0)
			return true
		end,
		OnUpdateState = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,state)
			C_PlaySoundSE(self.se[2],0,0)
			if ex1~=0xff then
				C_UpdateCharaPos(ex1,px,py+8)	-- 移動先の弾なら削除と同時に移動
			end
			C_RemoveBullet(blt_no,0)
		end,
		OnHitChara = function(self,scr_id,blt_type,blt_chr_no,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
			return 0 -- このイベント時点で弾のベクトルを変える場合は1を返す
		end,
		OnHitStage = function(self,scr_id,blt_type,blt_chr_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
			return 0 -- このイベント時点で弾のベクトルを変える場合は1を返す
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			if frame==1 then
				effect_no = C_AddEffect(scr_id,0,370,96,466,px,py,ex2*FPSO*1.5)
				if effect_no ~= -1 then
					C_SetEffectRotation(effect_no,5)
				end
			end
			return
		end,
	}
	setmetatable(spellcard,{ __index = bc.BaseBullet.new()})
	
	self = {
		id = CharaID,
		name = "フランドール・スカーレット",
		tex_chr = "data/scr/chr/010_fran/fran.png",
		angle_range_min = 45,
		angle_range_max = 90,
		move = 80,
		delay = 525,
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
		tex_face_fine_msg = "きゅっとしてドカーン！",
		tex_face_normal_msg = "また私と遊んでくれるのかしら？",
		tex_face_hurt_x = 0,
		tex_face_hurt_y = 256,
		tex_face_hurt_msg = "もう、煙も出ません…",
		blt = {blt1,blt2,blt0,blt3,blt4},
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
					effect_no = C_AddBGEffect(self.id,256,0,512,440,bgx,50,50)
					if effect_no ~= -1 then
						C_SetBGEffectFadeInOut(effect_no,6)
						C_SetBGEffectScale(effect_no,2.3,2.3)
						C_SetBGEffectVector(effect_no, 0,14,0,-0.2)
					end
					effect_no = C_AddEffect(self.id,128,338,256,466,px,py,45)
					if effect_no ~= -1 then
						C_SetEffectFadeInOut(effect_no,10)
						C_SetEffectRotation(effect_no,4)
						C_SetEffectVector(effect_no,2,2,0,0)
					end
					effect_no = C_AddEffect(self.id,128,338,256,466,px,py,45)
					if effect_no ~= -1 then
						C_SetEffectFadeInOut(effect_no,10)
						C_SetEffectRotation(effect_no,4)
						C_SetEffectVector(effect_no,-2,-2,0,0)
					end
					effect_no = C_AddEffect(self.id,128,338,256,466,px,py,45)
					if effect_no ~= -1 then
						C_SetEffectFadeInOut(effect_no,10)
						C_SetEffectRotation(effect_no,-4)
						C_SetEffectVector(effect_no,2,-2,0,0)
					end
					effect_no = C_AddEffect(self.id,128,338,256,466,px,py,45)
					if effect_no ~= -1 then
						C_SetEffectFadeInOut(effect_no,10)
						C_SetEffectRotation(effect_no,-4)
						C_SetEffectVector(effect_no,-2,2,0,0)
					end
				elseif frame >= 55 then
					C_ShowStage()
					return true
				elseif frame > 4 and frame < 45 then
					e=math.random(1,3)
					if e == 3 then
						bgx = 600
						stgw = C_GetStageWidth()
						if (stgw/2) <= px then	-- ステージの右よりの場合は左側に表示
							bgx = 200
						end
						fx = math.random(0,240)+bgx-120
						fy = math.random(50,450)
						tex = math.random(1,3)
						effect_no = C_AddBGEffect(self.id,128,256,192,320,fx,fy,10)
						if effect_no ~= -1 then
							scale = math.random()*3+3
							C_SetEffectFadeInOut(effect_no,5)
							C_SetBGEffectRotate(effect_no, math.random(0,359))
							C_SetBGEffectAlpha(effect_no,math.random(32,64))
							C_SetBGEffectScale(effect_no,scale,scale)
							C_SetBGEffectRotation(effect_no, math.random(2,5))
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
			end
		end
	}
	self.blt_type_count = #self.blt
	self.blt_sel_count = 2
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

