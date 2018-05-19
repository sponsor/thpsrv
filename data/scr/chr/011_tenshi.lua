module("data.scr.chr.011_tenshi",package.seeall)

-- 東方流星群キャラ追加データ「比那名居天子」
-- author:ごらんのすぽんさー
--
-- 【追加データ】
-- スクリプト：011_tenshi.lua
-- 画像：tenko.png
-- 音声：tenko_b10.wav,tenko_b11.wav,tenko_b20.wav,tenko_b21.wav,tenko_b30.wav,tenko_b31.wav,tenko_s00.wav,tenko_s01.wav,tenko_s02.wav
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
	local CharaID = 11
	local bc = require("data.scr.BaseChara")
	local blt1  = {
		id = CharaID,
		hit_range = 12,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 27,
		atk = 130,
		delay = 65,
		icon_x = 96,
		icon_y = 64,
		tex_x = 96,
		tex_y = 64,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/011_tenshi/tenko_b10.wav",
				"data/scr/chr/011_tenshi/tenko_b11.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			for i=0,8 do
				effect_no = C_AddEffect(self.id,16,80,32,96,bx,by,45)
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
			C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,OBJ_TYPE_SOLID,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,0,0)
			C_PlaySoundSE(self.se[1],0,0)
			return true
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			C_UpdateBulletAngle(blt_no,math.sin(math.rad(frame*10))*60)
		end,
	}
	setmetatable(blt1,{ __index = bc.BaseBullet.new()})
	
	local blt2 = {
		id = CharaID,
		hit_range = 8,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 39,
		atk = 220,
		delay = 150,
		icon_x = 128,
		icon_y = 64,
		tex_x = 128,
		tex_y = 64,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/011_tenshi/tenko_b20.wav",
				"data/scr/chr/011_tenshi/tenko_b21.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			effect_no = C_AddEffect(self.id,128,96,194,160,bx,by,40)
			if effect_no ~= -1 then
				C_SetEffectFade(effect_no,-8)
				C_SetEffectScale(effect_no,2,2)
			end
			for i=0,12 do
				effect_no = C_AddEffect(self.id,192,96,256,160,bx,by,30)
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
			blt_no = C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,OBJ_TYPE_SOLID,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,0,0)
			C_PlaySoundSE(self.se[1],0,0)
			return true
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			C_UpdateBulletAngle(blt_no,frame*20)
		end,
	}
	setmetatable(blt2,{ __index = bc.BaseBullet.new()})
	
	local blt3  = {
		id = CharaID,
		hit_range = 2,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 22,
		atk = 330,
		delay = 135,
		icon_x = 160,
		icon_y = 64,
		tex_x = 0,
		tex_y = 96,
		tex_w = 96,
		tex_h = 32,
		se = {	"data/scr/chr/011_tenshi/tenko_b30.wav",
				"data/scr/chr/011_tenshi/tenko_b31.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			for i=0,12 do
				effect_no = C_AddEffect(self.id,192,160,256,224,bx,by,30)
				if effect_no ~= -1 then
					ang = math.random(0,359)
					evx = math.cos(math.rad(ang))
					evy = math.sin(math.rad(ang))
					rnd = math.random(15,30)*0.05
					C_SetEffectScale(effect_no,rnd,rnd)
					C_SetEffectFade(effect_no,-math.random(8,16))
					vs = (math.random()*8+4)
					C_SetEffectVector(effect_no, vs*evx,vs*evy,-evx*rnd*0.15,(-evy*rnd*0.05)+0.2)
				end
			end
		end,
		Shot = function(self,chr_obj_no,chr_id,blt_type,px,py,vx,vy,vec_angle,power,frame)
			C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,OBJ_TYPE_SOLID,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,0,0)
			C_PlaySoundSE(self.se[1],0,0)
			return true
		end,
	}
	setmetatable(blt3,{ __index = bc.BaseBullet.new()})
	
	local spellcard = {
		id = CharaID,
		name = "要石「天地開闢プレス」",
		exp = 100,
		exp_max = 650,
		hit_range = 14,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 53,
		atk = 340,
		delay = 150,
		icon_x = 192,
		icon_y = 64,
		tex_x = 224,
		tex_y = 64,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/011_tenshi/tenko_s00.wav",
				"data/scr/chr/011_tenshi/tenko_s01.wav",
				"data/scr/chr/011_tenshi/tenko_s02.wav"},
		Spell = function(self,chr_obj_no,chr_id,px,py,vx,vy,vec_angle,power,frame)
			C_CreateBullet(BLT_PROC_TYPE_SCR_SPELL,chr_obj_no,chr_id,DEF_BLT_TYPE_SPELL,OBJ_TYPE_SOLID,px,py,vx,vy,self.add_vec_x,20,self.hit_range,0,65536)
			C_PlaySoundSE(self.se[1],0,0)
			return true
		end,
		OnHitChara = function(self,scr_id,blt_type,blt_chr_no,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
			if ex1 == 0 then
				nmv = C_GetCharaState(hit_obj_no,CHARA_STATE_NOMOVE)
				if nmv == 0 then
					C_SetCharaState(hit_obj_no,CHARA_STATE_NOMOVE,1)
				end
			elseif ex2 ~= 65536 and py ~= ex2 then
				C_RemoveBullet(blt_no,0)
				C_BombObject(self.id,blt_type,blt_chr_no,blt_no,px,py)
				return 1
			end
			return 0
		end,
		OnHitStage = function(self,scr_id,blt_type,blt_chr_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
			if ex1 == 1 then
				blt = C_GetBulletInfo(blt_no)
				C_RemoveBullet(blt_no,0)
				C_BombObject(self.id,blt_type,blt_chr_no,blt_no,blt.ax,blt.ay)
				return 1
			else
				if ex2 == 65536 then
					-- 地面にあたったy位置をex2保存
					C_SetBulletExtData2(blt_no, hy)
				end
				C_UpdateBulletState(blt_no,DEF_STATE_WAIT)
				C_UpdateBulletPosition(blt_no,hx,hy,0,0,0,0)
			end
			return 1
		end,
		OnTurnStart = function(self,blt_type,blt_chr_no,blt_no,turn,ex1,ex2,act_obj_no)
			blt = C_GetBulletInfo(blt_no)
			if ex2 ~= 65536 and ex2 ~= blt.ay then
				C_RemoveBullet(blt_no,0)
				C_BombObject(self.id,blt_type,blt_chr_no,blt_no,blt.ax,blt.ay)
			end
		end,
		OnUpdateState = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,state)
			if state == DEF_STATE_WAIT and ex1 == 0 then
				C_SetBulletExtData1(blt_no, 1)
				C_UpdateBulletAngle(blt_no,C_GetAngle(vx,vy))
				C_UpdateObjectType(blt_no,OBJ_TYPE_LIQUID)
				C_PlaySoundSE(self.se[2],0,0)
			end
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			if ex1 == 0 then
				C_UpdateBulletAngle(blt_no,vec_angle)
			end
		end,
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[3],0,0)
			for i=0,8 do
				effect_no = C_AddEffect(self.id,192,224,256,288,bx,by,30)
				if effect_no ~= -1 then
					ang = math.random(0,359)
					evx = math.cos(math.rad(ang))
					evy = math.sin(math.rad(ang))
					rnd = math.random(15,30)*0.05
					C_SetEffectScale(effect_no,rnd,rnd)
					C_SetEffectFade(effect_no,-math.random(8,16))
					vs = (math.random()*8+4)
					C_SetEffectVector(effect_no, vs*evx,vs*evy,-evx*rnd*0.15,(-evy*rnd*0.05)+0.2)
				end
			end
			for i=0,16 do
				effect_no = C_AddEffect(self.id,192,160,256,224,bx,by,40)
				if effect_no ~= -1 then
					ang = math.random(0,359)
					evx = math.cos(math.rad(ang))
					evy = math.sin(math.rad(ang))
					rnd = math.random(15,30)*0.05
					C_SetEffectScale(effect_no,rnd,rnd)
					C_SetEffectFade(effect_no,-math.random(6,10))
					vs = (math.random()*5+2)
					C_SetEffectVector(effect_no, vs*evx,vs*evy,-evx*rnd*0.15,-evy*rnd*0.15)
				end
			end
		end,
		OnHitCharaBulletBomb = function(self,hit_chr_no,blt_chr_no,blt_no,hx,hy,power)
			-- パワーアップ状態の時はアップ分を減算
			pw = C_GetCharaState(blt_chr_no,CHARA_STATE_POWER)
			if pw >= 1 then
				power = power * 0.77
			end
			-- 威力の下限を35%にする
			if power < 0.35 then
				power = 0.35
			end
			C_DamageCharaHP(blt_chr_no,hit_chr_no,math.ceil(-self.atk*power))
		end,
	}
	setmetatable(spellcard,{ __index = bc.BaseBullet.new()})
	
	self = {
		id = CharaID,
		name = "比那名居天子",
		tex_chr = "data/scr/chr/011_tenshi/tenko.png",
		angle_range_min = 10,
		angle_range_max = 100,
		move = 80,
		delay = 530,
		max_hp = 860,
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
		tex_face_fine_msg = "地上の生き物なんて相手にもならないわ",
		tex_face_normal_msg = "ふう、退屈しのぎにはなったわね",
		tex_face_hurt_x = 0,
		tex_face_hurt_y = 256,
		tex_face_hurt_msg = "天人の私がぁ～",
		blt = {blt1,blt2,blt3},
		sc = spellcard,
		se = {	"data/se/spell00.wav"},
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
					effect_no = C_AddBGEffect(self.id,256,0,512,338,bgx,130,50)
					if effect_no ~= -1 then
						C_SetBGEffectFadeInOut(effect_no,6)
						C_SetBGEffectScale(effect_no,2.2,2.2)
						C_SetBGEffectVector(effect_no, 0,6,0,-0.05)
					end
					effect_no = C_AddBGEffect(self.id,97,257,175,511,bgx+16,340,50)
					if effect_no ~= -1 then
						C_SetBGEffectAlpha(effect_no,0)
						C_SetBGEffectFade(effect_no,8)
						C_SetBGEffectFadeInOut(effect_no,5)
						C_SetBGEffectScalling(effect_no,0.1,0.05)
						C_SetBGEffectVector(effect_no, 0.5,5,0,-0.05)
					end
				elseif frame == 20 then
					effect_no = C_AddEffect(self.id,0,416,96,512,px,py,35)
					if effect_no ~= -1 then
						C_SetEffectFade(effect_no,-7)
						C_SetEffectScalling(effect_no,-0.1,-0.1)
						C_SetEffectRotation(effect_no, 40)
					end
				elseif frame >= 55 then
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

