module("data.scr.chr.014_yoshika",package.seeall)

-- 東方流星群キャラ追加データ「宮古芳香」
-- author:ごらんのすぽんさー
--
-- 【追加データ】
-- スクリプト：014_yoshika.lua
-- 画像：yoshika.png
-- 音声：yoshika_b10.wav,yoshika_b11.wav,yoshika_b20.wav,yoshika_b21.wav,yoshika_s00.wav,yoshika_s01.wav
--
-- 効果音はマッチメイカァズ様の音声を使用させていただきました。
--
-- 【再配布について】
-- このデータのスクリプト及び画像は再配布不可とします。

-- chara
Chara = {}

function Chara.new()
	local CharaID = 14
	local bc = require("data.scr.BaseChara")
	local blt1  = {
		id = CharaID,
		hit_range = 8,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 33,
		atk = 270,
		delay = 130,
		icon_x = 96,
		icon_y = 32,
		tex_x = 96,
		tex_y = 32,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/014_yoshika/yoshika_b10.wav",
				"data/scr/chr/014_yoshika/yoshika_b11.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			for i=0,16 do
				effect_no = C_AddEffect(self.id,96,32,128,64,bx,by,35)
				if effect_no ~= -1 then
					ang = math.random(0,359)
					evx = math.cos(math.rad(ang))
					evy = math.sin(math.rad(ang))
					rnd = math.random(20,30)*0.03
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
			effect_no = C_AddEffect(scr_id,96,32,128,64,px,py-8,3)
			if effect_no ~= -1 then
				ang = math.random(0,60)+60
				evx = math.cos(math.rad(ang))
				rnd = math.random()*0.3+0.6
				C_SetEffectScale(effect_no,rnd,rnd)
				C_SetEffectFade(effect_no,-10)
				C_SetEffectScalling(effect_no,-0.08,-0.08)
				vs = (math.random()*4+6)
				C_SetEffectVector(effect_no, vs*evx,vs,-evx*0.05,-5)
			end
		end,
	}
	setmetatable(blt1,{ __index = bc.BaseBullet.new()})
	
	local blt2 = {
		id = CharaID,
		hit_range = 5,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 27,
		atk = 130,
		delay = 150,
		icon_x = 128,
		icon_y = 32,
		tex_x = 128,
		tex_y = 64,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/014_yoshika/yoshika_b20.wav",
				"data/scr/chr/014_yoshika/yoshika_b21.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			effect_no = C_AddEffect(self.id,256,129,288,159,bx,by,16)
			if effect_no ~= -1 then
				C_SetEffectScale(effect_no,2,2)
				C_SetEffectAnimation(effect_no,2,8,false)
			end
		end,
		Shot = function(self,chr_obj_no,chr_id,blt_type,px,py,vx,vy,vec_angle,power,frame)
			C_PlaySoundSE(self.se[1],0,0)
			C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,vx,vy,0,self.add_vec_y,self.hit_range,0,0)
			return true
		end,
		OnHitCharaBulletBomb = function(self,hit_chr_no,blt_chr_no,blt_no,hx,hy,power)
			pain = C_GetCharaState(hit_chr_no,CHARA_STATE_PAIN)
			C_SetCharaState(hit_chr_no, CHARA_STATE_PAIN, pain+13)
--			C_SetCharaState(hit_chr_no, CHARA_STATE_PAIN,13)
			-- ダメージ
			C_DamageCharaHP(blt_chr_no,hit_chr_no,-self.atk*power)
		end
	}
	setmetatable(blt2,{ __index = bc.BaseBullet.new()})
	
	local spellcard = {
		id = CharaID,
		name = "回復「ヒールバイデザイア」",
		exp = 0,
		exp_max = 790,
		hit_range = 12,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 24,
		atk = 100,
		delay = 50,
		icon_x = 160,
		icon_y = 32,
		tex_x = 0,
		tex_y = 129,
		tex_w = 32,
		tex_h = 30,
		se = {	"data/scr/chr/014_yoshika/yoshika_s00.wav",
				"data/scr/chr/014_yoshika/yoshika_s01.wav"},
		Spell = function(self,chr_obj_no,chr_id,px,py,vx,vy,vec_angle,power,frame)
			if (frame % 2) == 0 then
				t = frame/2
				chr = C_GetCharacterFromObjNo(chr_obj_no)
				rx = math.cos(math.rad(t*60))
				ry = math.sin(math.rad(t*60))
				C_CreateBullet(BLT_PROC_TYPE_SCR_SPELL,chr_obj_no,chr_id,DEF_BLT_TYPE_SPELL,OBJ_TYPE_STAGE,chr.ax+rx*30,chr.ay+ry*30,0,0,rx*5,ry*5,self.hit_range,0,1)
				if t >= 5 then
					C_PlaySoundSE(self.se[1],0,0)
					return true
				end
			end
			return false
		end,
		OnFrame = function(self,blt_type,blt_no,frame,px,py,vx,vy,ex1,ex2)
			if ex1==0 and frame > FPS then
				C_UpdateBulletState(blt_no,DEF_STATE_WAIT)
			elseif ex1==2 and ((frame+1)%FPS) == 0 then
				blt = C_GetBulletInfo(blt_no)
				chr = C_GetCharacterFromObjNo(blt.chr_obj_no)
				ang = C_GetAngle(chr.ax-blt.ax,chr.ay-blt.ay)
				C_UpdateBulletVector(blt_no,math.cos(math.rad(ang))*300,math.sin(math.rad(ang))*300,0,0)
			end
		end,
		OnUpdateState = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,state)
			if state==DEF_STATE_WAIT then
				C_SetBulletExtData1(blt_no, 1)
				C_UpdateBulletVector(blt_no,0,0,0,0)
			end
		end,
		OnHitBullet = function(self,scr_id,blt_type,blt_chr_no,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
			if C_RemoveBullet(blt_no,0) == true then
				hb = C_GetBulletInfo(hit_obj_no)
				hc_no = tonumber(hb.chr_obj_no)
				C_UpdateCharaStatus(hc_no,50,0,0,0)
			end
			return 1
		end,
		OnHitChara = function(self,scr_id,blt_type,blt_chr_no,hit_chr_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
			if C_RemoveBullet(blt_no,2) == true then
				C_PlaySoundSE(self.se[2],0,0)
				C_UpdateCharaStatus(hit_chr_no,50,0,0,0)
			end
			return 1
		end,
		OnTurnEnd = function(self,blt_type,blt_chr_no,blt_no,turn,ex1,ex2,act_obj_no)
			if blt_chr_no == act_obj_no then
				-- キャラの状態を確認
				chr = C_GetCharacterFromObjNo(blt_chr_no)
				if bit.band(chr.obj_state,DEF_STATE_DEAD) > 0 or bit.band(chr.obj_state,DEF_STATE_DROP) > 0 then
					return
				end
				
				if ex2 == 1 then
					C_SetBulletExtData2(blt_no, 0)
				else
					C_SetBulletExtData1(blt_no, 2)
					chr = C_GetCharacterFromObjNo(blt_chr_no)
					blt = C_GetBulletInfo(blt_no)
					ang = C_GetAngle(chr.ax-blt.ax,chr.ay-blt.ay)
					C_UpdateBulletVector(blt_no,math.cos(math.rad(ang))*300,math.sin(math.rad(ang))*300,0,0)
					C_UpdateBulletState(blt_no,DEF_STATE_ACTIVE)
				end
			end
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			if (frame % 3) == 0 then
				C_SetBulletTextureIndex(blt_no, (frame/3)%8)
			end
		end,
	}
	setmetatable(spellcard,{ __index = bc.BaseBullet.new()})
	
	self = {
		id = CharaID,
		name = "宮古芳香",
		tex_chr = "data/scr/chr/014_yoshika/yoshika.png",
		angle_range_min = 35,
		angle_range_max = 115,
		move = 90,
		delay = 527,
		max_hp = 890,
		draw_w = 45,
		draw_h = 45,
		shot_h = 9,
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
		tex_face_fine_msg = "ゾンビは永遠に不滅です！",
		tex_face_normal_x = 320,
		tex_face_normal_y = 32,
		tex_face_normal_msg = "またやるなら起こしてねー",
		tex_face_hurt_x = 416,
		tex_face_hurt_y = 32,
		tex_face_hurt_msg = "うおー\nやーらーれーたー",
		blt = {blt1,blt2},
		sc = spellcard,
		se = {	"data/se/spell00.wav"},
		OnDeadChara = function(self,chr_no,type,prv_stt)
			if type ~= DEF_CHARA_DEAD_DROP then
				chr = C_GetCharacterFromObjNo(chr_no)
				if chr~=nil then
					if chr.angle <= 90 or chr.angle >= 270 then		
						C_SetCharaTexture(chr_no,32,32,64,64)
					else
						C_SetCharaTexture(chr_no,64,32,96,64)
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
					vgx = 1.0
					stgh = C_GetStageWidth()/2
					if stgh <= px then	-- ステージの右よりの場合は左側に表示
						vgx = -1.0
					end
					effect_no = C_AddBGEffect(self.id,64,160,512,512,224*-vgx+stgh,250,50)
					if effect_no ~= -1 then
						C_SetBGEffectFadeInOut(effect_no,6)
						C_SetBGEffectScale(effect_no,1.5*vgx,1.5)
						C_SetBGEffectVector(effect_no, 5*vgx,0,-0.02*vgx,0)
					end
					for i=0,20 do
						effect_no = C_AddEffect(self.id,0,160,64,224,(vgx*(math.random()*256))+stgh-112*vgx,math.random()*440,50)
						if effect_no ~= -1 then
							C_SetEffectFadeInOut(effect_no,6)
							rnd = math.random(5,20)*0.1
							C_SetEffectScale(effect_no,rnd,rnd)
--							C_SetEffectScalling(effect_no,-0.015,-0.015)
							C_SetEffectRotation(effect_no, math.random(12,24))
							C_SetEffectVector(effect_no,vgx*math.random(2,4),0,vgx*math.random(10,20)*0.01,0)
						end
					end
				elseif frame == 15 then
					effect_no = C_AddEffect(self.id,0,224,64,288,px,py,25)
					if effect_no ~= -1 then
						C_SetEffectFade(effect_no,-10)
						C_SetEffectScale(effect_no,2,2)
						C_SetEffectScalling(effect_no,-0.1,-0.1)
						C_SetEffectRotation(effect_no, 40)
					end
				elseif frame >= 55 then
					C_ShowStage()
					return true
				elseif frame > 15 then
					vs = (math.random()*6+2)
					rad = math.rad(math.random(0,359))
					evx = math.cos(rad)
					evy = math.sin(rad)
					effect_no = C_AddEffect(self.id,0,64,64,128,px+vs*evx*25,py+vs*evy*25,20)
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
