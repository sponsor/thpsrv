module("data.scr.chr.001_marisa",package.seeall)

-- chara
Chara = {}

function Chara.new()
	local CharaID = 1
	local bc = require("data.scr.BaseChara")
	local blt1  = {
		id = CharaID,
		hit_range = 6,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 22,
		atk = 84,
		delay = 140,
		icon_x = 96,
		icon_y = 64,
		tex_x = 96,
		tex_y = 64,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/001_marisa/marisa_b10.wav",
				"data/scr/chr/001_marisa/marisa_b11.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			effect_no = C_AddEffect(self.id,64,96,128,160,bx,by,30)
			if effect_no ~= -1 then
				C_SetEffectFade(effect_no,-10)
			end
		end,
		Shot = function(self,chr_obj_no,chr_id,blt_type,px,py,vx,vy,vec_angle,power,frame)
			if frame==0 then
				C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,vx,vy,self.add_vec_x,self.add_vec_y-4,self.hit_range,1,1)
				C_PlaySoundSE(self.se[1],0,0)
			elseif frame == 4 then
				C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,1,1)
				C_PlaySoundSE(self.se[1],0,0)
			elseif frame == 8 then
				C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,vx,vy,self.add_vec_x,self.add_vec_y+4,self.hit_range,1,1)
				C_PlaySoundSE(self.se[1],0,0)
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
		hit_range = 8,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 32,
		atk = 280,
		delay = 130,
		icon_x = 128,
		icon_y = 64,
		tex_x = 128,
		tex_y = 64,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/001_marisa/marisa_b20.wav",
				"data/scr/chr/001_marisa/marisa_b21.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			effect_no = C_AddEffect(self.id,224,64,256,96,bx,by,50)
			if effect_no ~= -1 then
				C_SetEffectScale(effect_no,2,2)
				C_SetEffectFade(effect_no,-8)
			end
			e=math.random(8,20)
			for i=0,e do
				effect_no = C_AddEffect(self.id,128,64,160,96,bx,by,25)
				if effect_no ~= -1 then
					ang = math.random(0,359)
					evx = math.cos(math.rad(ang))
					evy = math.sin(math.rad(ang))
					rnd = math.random(40,75)*0.01
					C_SetEffectScale(effect_no,rnd,rnd)
					C_SetEffectRotation(effect_no, 10)
					C_SetEffectFade(effect_no,-math.random(10,20))
					vs = (math.random()*8+4)
					C_SetEffectVector(effect_no, vs*evx,vs*evy,-evx*rnd*0.5,-evy*rnd*0.5+0.02)
				end
			end
		end,
		Shot = function(self,chr_obj_no,chr_id,blt_type,px,py,vx,vy,vec_angle,power,frame)
			C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,1,1)
			C_PlaySoundSE(self.se[1],0,0)
			return true
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			if math.random(0,1) == 1 then
				ang = vec_angle+math.random(0,60)-30
				evx = -math.cos(math.rad(ang))
				evy = -math.sin(math.rad(ang))
				effect_no = C_AddEffect(scr_id,128,64,160,96,px,py,25)
				if effect_no ~= -1 then
					C_SetEffectScale(effect_no,0.5,0.5)
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
	}
	setmetatable(blt2,{ __index = bc.BaseBullet.new()})
	local blt3 = {
		id = CharaID,
		hit_range = 10,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 50,
		atk = 105,
		delay = 200,
		icon_x = 160,
		icon_y = 64,
		tex_x = 160,
		tex_y = 64,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/001_marisa/marisa_b30.wav",
				"data/scr/chr/001_marisa/marisa_b31.wav"},
		Shot = function(self,chr_obj_no,chr_id,blt_type,px,py,vx,vy,vec_angle,power,frame)
			C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,1,1)
			C_PlaySoundSE(self.se[1],0,0)
			return true
		end,
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			effect_no = C_AddEffect(self.id,1,97,63,159,bx,by,30)
			if effect_no ~= -1 then
				C_SetEffectFade(effect_no,-10)
				C_SetEffectScalling(effect_no,0.20,0.20)
				C_SetEffectRotation(effect_no, 10)
			end
		end,
	}
	setmetatable(blt3,{ __index = bc.BaseBullet.new()})

	local spellcard = {
		id = CharaID,
		name = "恋符「マスタースパーク」",
		exp = 700,
		exp_max = 700,
		hit_range = 9,
		add_vec_x = 0,
		add_vec_y = 0,
		bomb_range= 22,
		atk = 100,
		delay = 240,
		icon_x = 192,
		icon_y = 64,
		tex_x = 224,
		tex_y = 64,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/001_marisa/marisa_s00.wav"},--,
		Spell = function(self,chr_obj_no,chr_id,px,py,vx,vy,vec_angle,power,frame)
			wind = C_GetWindValue()
			blt_no = C_CreateBullet(BLT_PROC_TYPE_SCR_SPELL,chr_obj_no,chr_id,DEF_BLT_TYPE_SPELL,OBJ_TYPE_SOLID,px,py,vx,vy,-wind,0,self.hit_range,wind,0)
			C_PlaySoundSE(self.se[1],0,0)
			return true
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			effect_no = C_AddEffect(scr_id,224,64,256,96,px,py,50)
			if effect_no ~= -1 then
				C_SetEffectFade(effect_no, -6)
			end
		end,
		OnFrame = function(self,type,blt_no,frame,px,py,vx,vy,ex1,ex2)
			if frame >= 90 then
				C_RemoveBullet(blt_no,0)
			end
		end,
		OnHitCharaBulletBomb = function(self,hit_chr_no,blt_chr_no,blt_no,hx,hy,power)
			return
		end,
		OnHitChara = function(self,scr_id,blt_type,blt_chr_no,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
			ex2 = C_GetBulletExtData2(blt_no)
			chrbit = bit.lshift(1,hit_obj_no)
			if bit.band(ex2, chrbit) == 0 then	-- キャラのビットが立っていない場合
				C_DamageCharaHP(blt_chr_no,hit_obj_no,-self.atk)
				C_SetBulletExtData2(blt_no, bit.bor(chrbit,ex2))	-- ビットを立てて保存
			end
			return 0 -- このイベント時点で弾のベクトルを変える場合は1を返す
		end,
		OnHitStage = function(self,scr_id,blt_type,blt_chr_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
			C_BombObject(scr_id,blt_type,blt_chr_no,blt_no,px,py)
			return 0 -- このイベント時点で弾のベクトルを変える場合は1を返す
		end,
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			if pxls==0 then	-- 削除ピクセル0ならエフェクト表示しない
				return
			end
			e=math.random(2,4)
			for i=0,e do
				effect_no = C_AddEffect(self.id,128,64,160,96,bx,by,25)
				if effect_no ~= -1 then
					ang = math.random(0,359)
					evx = math.cos(math.rad(ang))
					evy = math.sin(math.rad(ang))
					rnd = math.random(40,75)*0.01
					C_SetEffectScale(effect_no,rnd,rnd)
					C_SetEffectRotation(effect_no, 10)
					C_SetEffectFade(effect_no,-math.random(10,20))
					vs = (math.random()*8+4)
					C_SetEffectVector(effect_no, vs*evx,vs*evy,-evx*rnd*0.5,-evy*rnd*0.5+0.02)
				end
			end
		end,
	}
	setmetatable(spellcard,{ __index = bc.BaseBullet.new()})
	
	self = {
		id = CharaID,
		name = "霧雨魔理沙",
		tex_chr = "data/scr/chr/001_marisa/marisa.png",
		angle_range_min = 20,
		angle_range_max = 75,
		move = 70,
		delay = 555,
		max_hp = 770,
		draw_w = 45,
		draw_h = 45,
		shot_h = 8,
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
		tex_face_hurt_x = 96,
		tex_face_hurt_y = 256,
		tex_face_fine_msg = "わたしに掛かればこんなもんだぜ",
		tex_face_normal_msg = "調子良いと思ったのになぁ",
		tex_face_hurt_msg = "・・・さあ帰ろ",
		blt = {blt1, blt2, blt3},
		sc = spellcard,
		se = {	"data/se/spell00.wav"},
		OnTriggerFrame = function(self,type,px,py,angle,frame)
			if type == DEF_BLT_TYPE_SPELL then	-- スペルカード演出
				if frame == 0 then
					C_HideStage()
				elseif frame >= 45 then
					C_ShowStage()
					return true
				elseif frame == 4 then
					bgx = 320
					scalex = -1.0
					sallingx = -0.09
					vecx = -12
					if angle <= 90 or angle >= 270 then	-- 左を向いている場合
						bgx = 480
						scalex = 1.0
						sallingx = 0.09
						vecx = 12
					end
					effect_no = C_AddBGEffect(self.id,256,0,512,400,bgx,320,40)
					if effect_no ~= -1 then
						C_SetBGEffectAlpha(effect_no,0)
						C_SetBGEffectFade(effect_no,10)
						C_SetBGEffectScale(effect_no,scalex, 1.0)
						C_SetBGEffectScalling(effect_no, sallingx, 0.09,30)
						C_SetBGEffectVector(effect_no,vecx,0,0,0,30)
					end
				elseif frame == 15 then
					C_PlaySoundSE(self.se[1],0,0)
					effect_no = C_AddEffect(self.id,0,256,96,352,px,py,25)
					if effect_no ~= -1 then
						C_SetEffectFade(effect_no,-10)
						C_SetEffectScalling(effect_no,-0.1,-0.1)
						C_SetEffectRotation(effect_no, 40)
					end
				elseif frame == 30 then
					effect_no = C_AddBGEffect(self.id,0,256,96,352,400,100,15)
					if effect_no ~= -1 then
						C_SetBGEffectAlpha(effect_no,0)
						C_SetBGEffectFade(effect_no,10)
						C_SetBGEffectScale(effect_no,4,4)
					end
				elseif frame > 15 then
					vs = (math.random()*6+2)
					rad = math.rad(math.random(0,359))
					evx = math.cos(rad)
					evy = math.sin(rad)
					effect_no = C_AddEffect(self.id,96,64,128,96,px+vs*evx*25,py+vs*evy*25,20)
					if effect_no ~= -1 then
						rad = math.random()
						rnd = math.random()+0.5
						C_SetEffectScale(effect_no,rnd,rnd)
						C_SetEffectFade(effect_no,-12)
						C_SetEffectVector(effect_no, -vs*evx,-vs*evy,vs*evx*0.01,vs*evy*0.01)
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
		end
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
	return setmetatable( self , { __index = bc.BaseChara.new() })
end

