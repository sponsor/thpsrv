module("data.scr.chr.007_eiki",package.seeall)

-- chara
Chara = {}

function Chara.new()
	local CharaID = 7
	local bc = require("data.scr.BaseChara")
	local blt1  = {
		id = CharaID,
		hit_range = 4,
		add_vec_x = 0,
		add_vec_y = 0,
		bomb_range= 19,
		atk = 80,
		delay = 150,
		icon_x = 96,
		icon_y = 64,
		tex_x = 0,
		tex_y = 96,
		tex_w = 64,
		tex_h = 32,
		se = {	"data/scr/chr/007_eiki/eiki_b10.wav",
				"data/scr/chr/007_eiki/eiki_b11.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			effect_no = C_AddEffect(self.id,64,96,128,160,bx,by,40)
			if effect_no ~= -1 then
				C_SetEffectScale(effect_no,2,2)
				C_SetEffectFade(effect_no,-6)
			end
			if pxls == 0 then
				C_RemoveBullet(blt_no,0)
				return
			end
		end,
		Shot = function(self,chr_obj_no,chr_id,blt_type,px,py,vx,vy,vec_angle,power,frame)
			wind = C_GetWindValue()
			blt_no = C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,vx,vy,-wind,0,self.hit_range,4,0)
			C_PlaySoundSE(self.se[1],0,0)
			return true
		end,
		OnFrame = function(self,type,blt_no,frame,px,py,vx,vy,ex1,ex2)
			if frame >= 30 then
				C_RemoveBullet(blt_no,0)
			end
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			effect_no = C_AddEffect(scr_id,0,96,64,128,px,py,50)
			if effect_no ~= -1 then
				C_SetEffectFade(effect_no, -6)
				C_SetEffectRotate(effect_no, vec_angle)
			end
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
			if ex1 == 0 then
				C_RemoveBullet(blt_no,0)
				C_BombObject(scr_id,blt_type,blt_chr_no,blt_no,px,py)
			else
				C_SetBulletExtData1(blt_no, ex1-1)
				C_UpdateBulletVector(blt_no,vx,vy,0,0)	-- クライアント側の位置補正
				C_BombObject(scr_id,blt_type,blt_chr_no,blt_no,px,py)
				return 0
			end
			return 1 -- このイベント時点で弾のベクトルを変える場合は1を返す
		end,
	}
	setmetatable(blt1,{ __index = bc.BaseBullet.new()})
	
	local blt2 = {
		id = CharaID,
		hit_range = 5,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 37,
		atk = 240,
		delay = 135,
		icon_x = 128,
		icon_y = 64,
		tex_x = 128,
		tex_y = 64,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/007_eiki/eiki_b20.wav",
				"data/scr/chr/007_eiki/eiki_b21.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			effect_no = C_AddEffect(self.id,64,96,128,160,bx,by,40)
			if effect_no ~= -1 then
				C_SetEffectFade(effect_no,-8)
				C_SetEffectScale(effect_no,4,4)
			end
			for i=0,12 do
				effect_no = C_AddEffect(self.id,64,96,128,160,bx,by,30)
				if effect_no ~= -1 then
					ang = math.random(0,359)
					evx = math.cos(math.rad(ang))
					evy = math.sin(math.rad(ang))
					rnd = math.random(15,30)*0.05
					C_SetEffectScale(effect_no,rnd,rnd)
					C_SetEffectFade(effect_no,-math.random(8,16))
					vs = (math.random()*8+4)
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
			if vx > 0 then
				C_UpdateBulletAngle(blt_no,frame*32)
			else
				C_UpdateBulletAngle(blt_no,frame*32)
			end
		end,
	}
	setmetatable(blt2,{ __index = bc.BaseBullet.new()})
	
	local blt0 = {
		id = CharaID,
		hit_range = 14,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 44,
		atk = 240,
		delay = 160,
		icon_x = 224,
		icon_y = 64,
		tex_x = 224,
		tex_y = 128,
		tex_w = 32,
		tex_h = 128,
		se = {	"data/scr/chr/007_eiki/eiki_b02.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[1],0,0)
			effect_no = C_AddEffect(self.id,64,96,128,160,bx,by,40)
			if effect_no ~= -1 then
				C_SetEffectFade(effect_no,-8)
				C_SetEffectScale(effect_no,4,4)
			end
			for i=0,10 do
				effect_no = C_AddEffect(self.id,64,96,128,160,bx,by,30)
				if effect_no ~= -1 then
					ang = math.random(0,359)
					evx = math.cos(math.rad(ang))
					evy = math.sin(math.rad(ang))
					rnd = math.random(15,30)*0.05
					C_SetEffectScale(effect_no,rnd,rnd)
					C_SetEffectFade(effect_no,-math.random(8,16))
					vs = (math.random()*8+4)
					C_SetEffectVector(effect_no, vs*evx,vs*evy,-evx*rnd*0.15,0.3)
				end
			end
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			effect_no = C_AddEffect(self.id,128,368,192,496,px,py,50)
			if effect_no ~= -1 then
				C_SetEffectFade(effect_no, -6)
				C_SetEffectRotate(effect_no, vec_angle+90)
			end
		end,
		Shot = function(self,chr_obj_no,chr_id,blt_type,px,py,vx,vy,vec_angle,power,frame)
			C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,-48,0,2000,-wind,0,self.hit_range,0,frame)
			return true
		end,
		OnHitChara = function(self,scr_id,blt_type,blt_chr_no,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
			return 0 -- このイベント時点で弾のベクトルを変える場合は1を返す
		end,
		OnHitBullet= function(self,scr_id,blt_type,blt_chr_no,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
			if ex2==hit_obj_no then
				spl = C_GetBulletInfo(ex2)
				if C_RemoveBullet(blt_no,0) == true then -- obj_no,rm_type(0:normal/1:out/2:bomb)
					bx = bit.rshift(bit.band(ex1,0xffff0000),16)	--上位16bitがx、下位16bitがy
					by = bit.band(ex1,0x0000ffff)
					C_BombObject(7,DEF_BLT_TYPE_SPELL,blt_chr_no,ex2,bx,by)
				end
				return 1
			end
			return 0
		end,
	}
	setmetatable(blt0,{ __index = bc.BaseBullet.new()})

	local spellcard = {
		id = CharaID,
		name = "審判「ラストジャッジメント」",
		exp = 250,
		exp_max = 550,
		hit_range = 8,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 53,
		atk = 280,
		delay = 210,
		icon_x = 160,
		icon_y = 64,
		tex_x = 192,
		tex_y = 64,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/007_eiki/eiki_b20.wav",
				"data/scr/chr/007_eiki/eiki_s01.wav",
				"data/scr/chr/007_eiki/eiki_s02.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[3],0,0)
			effect_no = C_AddEffect(self.id,128,96,192,160,bx,by,40)
			if effect_no ~= -1 then
				C_SetEffectScale(effect_no,2,2)
				C_SetEffectScalling(effect_no,0.05,0.05)
				C_SetEffectFade(effect_no,-7)
				C_SetEffectRotation(effect_no,20)
			end
			for i=0,20 do
				effect_no = C_AddEffect(self.id,0,128,32,160,bx,by,30)
				if effect_no ~= -1 then
					ang = math.random(0,359)
					evx = math.cos(math.rad(ang))
					evy = math.sin(math.rad(ang))
					rnd = math.random(15,30)*0.05
					C_SetEffectScale(effect_no,rnd,rnd)
					C_SetEffectFade(effect_no,-math.random(8,16))
					vs = (math.random()*8+4)
					C_SetEffectVector(effect_no, vs*evx,vs*evy,-evx*rnd*0.15,0.3)
				end
			end
		end,
		Spell = function(self,chr_obj_no,chr_id,px,py,vx,vy,vec_angle,power,frame)
			C_CreateBullet(BLT_PROC_TYPE_SCR_SPELL,chr_obj_no,chr_id,DEF_BLT_TYPE_SPELL,OBJ_TYPE_SOLID,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,0,0)
			C_PlaySoundSE(self.se[1],0,0)
			return true
		end,
		OnHitChara = function(self,scr_id,blt_type,blt_chr_no,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
			ex1 = C_GetBulletExtData1(blt_no)
			chrbit = bit.lshift(1,hit_obj_no)
			if bit.band(ex1, chrbit) == 0 then	-- キャラのビットが立っていない場合
				C_DamageCharaHP(blt_chr_no,hit_obj_no,-50)
				C_SetBulletExtData1(blt_no, bit.bor(chrbit,ex1))	-- ビットを立てて保存
			end
			return 0 -- このイベント時点で弾のベクトルを変える場合は1を返す
		end,
		OnHitStage = function(self,scr_id,blt_type,blt_chr_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
			if ex2 == 0 then
				ex = 0x0
				ex = bit.lshift(hx,16)+hy	--上位16bitをx、下位16bitをyで保存
				wind = C_GetWindValue()
				C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,blt_chr_no,self.id,2,OBJ_TYPE_SOLID,px,0,0,2000,-wind,0,15,ex,blt_no)
				C_UpdateBulletVector(blt_no,0,0,-wind,0)
				C_SetBulletExtData2(blt_no, 1)
				C_UpdateBulletState(blt_no,DEF_STATE_WAIT)
				return 1
			end
			return 0 -- このイベント時点で弾のベクトルを変える場合は1を返す
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			if state ~= DEF_STATE_WAIT then
				C_UpdateBulletAngle(blt_no,vec_angle)
			end
		end,
		OnUpdateState = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,state)
			if state==DEF_STATE_WAIT then
				effect_no = C_AddEffect(self.id,0,416,96,512,px,0,50)
				if effect_no ~= -1 then
					C_SetEffectFade(effect_no,-5)
					C_SetEffectScale(effect_no,2,2)
				end
				C_PlaySoundSE(self.se[2])
			end
		end,
		OnTurnEnd = function(self,blt_type,blt_chr_no,blt_no,turn,ex1,ex2,act_obj_no)
			C_RemoveBullet(blt_no,0) -- obj_no,rm_type(0:normal/1:out/2:bomb)
		end,
	}
	setmetatable(spellcard,{ __index = bc.BaseBullet.new()})
	
	self = {
		id = CharaID,
		name = "四季映姫・ヤマザナドゥ",
		tex_chr = "data/scr/chr/007_eiki/eiki.png",
		angle_range_min = 15,
		angle_range_max = 75,
		move = 70,
		delay = 540,
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
		tex_face_fine_msg = "自らの罪を悔い改めるがいい！ 今日は罪の重さを理解するまで帰しません。",
		tex_face_normal_msg = "少しは善行を積んで罪は減らせているかしら？　最後は結局白黒つくだけですよ。",
		tex_face_hurt_x = 0,
		tex_face_hurt_y = 256,
		tex_face_hurt_msg = "そう、これは遊び。しかし必死になるからこそ遊びは楽しいものなのです。",
		blt = {blt2,blt1,blt0},
		sc = spellcard,
		se = {	"data/se/spell00.wav"},
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
					effect_no = C_AddBGEffect(self.id,256,0,512,368,bgx,100,45)
					if effect_no ~= -1 then
						C_SetBGEffectAlpha(effect_no,0)
						C_SetBGEffectFade(effect_no,10)
						C_SetBGEffectScale(effect_no,2,2)
						C_SetBGEffectVector(effect_no, 0,5,0,0)
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
					e=math.random(1,2)
					if e == 1 then
						fx = math.random(0,500)+bgx-250
						effect_no = C_AddBGEffect(self.id,192,256,256,512,fx,-5,40-frame)
						if effect_no ~= -1 then
							scale = math.random()+0.5
							C_SetBGEffectAlpha(effect_no,math.random(64,96))
							C_SetBGEffectScale(effect_no,scale,scale*2)
							C_SetBGEffectFadeInOut(effect_no,5)
							C_SetBGEffectVector(effect_no, math.random()*2-1,math.random(40,55),0,0)
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
--	local bc = require("data.scr.BaseChara")
	return setmetatable( self , { __index = bc.BaseChara.new() })
end

