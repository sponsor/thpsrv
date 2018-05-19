module("data.scr.BaseChara",package.seeall)

-- define --
-- bullet
BaseBullet = {}

function BaseBullet.new()
	local self = {
		name = "name",
		hit_range = 5,
		bomb_range= 30,
		add_vec_x = 0,
		add_vec_y = 20,
		atk = 1000,
		delay = 30,
		icon_x = 32,
		icon_y = 64,
		tex_x = 32,
		tex_y = 64,
		tex_w = 32,
		tex_h = 32,
		tex_count = 2,
		ephemeris = false,
	}
	return setmetatable(self, { __index = BaseBullet })
end

-- chara
BaseChara = {}

function BaseChara.new()
	local self = {
		id = 0,
		name = "name",
		tex_chr = "data/img/chr/base.png",
		send = true,
		angle_range_min = 0,
		angle_range_max = 90,
		move = 100,
		delay = 550,
		max_hp = 1000,
		draw_w = 32,
		draw_h = 32,
		shot_h = 6,
		tex_chr_num = 4,
		tex_chr_x = 0,
		tex_chr_y = 0,
		tex_chr_w = 32,
		tex_chr_h = 32,
		tex_gui_face_x = 0,
		tex_gui_face_y = 32,
		tex_trg_num = 4,
		tex_trg_x = 0,
		tex_trg_y = 32,
		tex_trg_w = 32,
		tex_trg_h = 32,
		tex_face_fine_x = 0,
		tex_face_fine_y = 160,
		tex_face_fine_msg = "良い結果でした",
		tex_face_normal_x = 96,
		tex_face_normal_y = 160,
		tex_face_normal_msg = "普通の結果でした",
		tex_face_hurt_x = 192,
		tex_face_hurt_y = 160,
		tex_face_hurt_msg = "悪い結果でした",
		tex_face_w = 96,
		tex_face_h = 96,
		blt = {},
		sc = {},
		se = {}
	}
	self.blt_type_count = #self.blt
	self.blt_sel_count = self.blt_type_count
	return setmetatable(self, { __index = BaseChara })
end

-- procedure --
-- bullet ########################
-- 弾の情報
function BaseChara:GetBltInfo(num)
	return self.blt[num+1]:GetInfo()
end
function BaseBullet:GetInfo()
	return self.atk, self.delay, self.icon_x, self.icon_y, self.tex_x, self.tex_y, self.tex_w, self.tex_h, self.hit_range,self.bomb_range,self.ephemeris
end

-- 弾作成
function BaseChara:ShotBullet(chr_obj_no,blt_type,px,py,vx,vy,vec_angle,power,frame,indicator_angle,indicator_power)
	return self.blt[blt_type+1]:Shot(chr_obj_no,self.id,blt_type,px,py,vx,vy,vec_angle,power,frame,indicator_angle,indicator_power)
end
-- powerに入る値はは0～MAX_SHOT_POWER
-- 1=SOLID,2=GAS,4=LIQUID,ret trueで発射終了,falseを返すとまた呼ばれframeがカウントアップしていく(最大250frame)
function BaseBullet:Shot(chr_obj_no,scr_id,blt_type,px,py,vx,vy,vec_angle,power,frame,indicator_angle,indicator_power)
	blt_no = C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,scr_id,blt_type,1,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,1,1)
	if blt_no ~= -1 then
		C_UpdateBulletAngle(blt_no,vec_angle)
	end
	return true
end
-- スペル作成
function BaseChara:ShotSpell(chr_obj_no,px,py,vx,vy,vec_angle,power,frame,indicator_angle,indicator_power)
	return self.sc:Spell(chr_obj_no,self.id,px,py,vx,vy,vec_angle,power,frame,indicator_angle,indicator_power)
end
-- 1=SOLID,2=GAS,4=LIQUID,ret trueで発射終了,falseを返すとまた呼ばれframeがカウントアップしていく(最大250frame)
function BaseBullet:Spell(chr_obj_no,scr_id,px,py,vx,vy,vec_angle,power,frame,indicator_angle,indicator_power)
	blt_no = C_CreateBullet(BLT_PROC_TYPE_SCR_SPELL,chr_obj_no,scr_id,0,1,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,1,1)
	if blt_no ~= -1 then
		C_UpdateBulletAngle(blt_no,vec_angle)
	end
	return true
end

-- 地面に当たった：弾タイプ,弾作ったキャラObjNo,obj_no,スクリプト番号,当たった位置x,y/移動値x,y/残り移動時間0.0～1.0/extdata
function BaseChara:OnHitStageBullet(blt_type,blt_chr_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
	return self.blt[blt_type+1]:OnHitStage(self.id,blt_type,blt_chr_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
end
function BaseBullet:OnHitStage(scr_id,blt_type,blt_chr_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
	C_RemoveBullet(blt_no,0) -- blt_no,rm_type(0:normal/1:out/2:bomb)
	C_BombObject(scr_id,blt_type,blt_chr_no,blt_no,hx,hy)
	return 1
end

-- キャラに当たった：弾作ったキャラObjNo,chr_no,obj_no,スクリプト番号,弾タイプ,当たった位置x,y/当たった箇所x,y/移動値x,y/残り移動時間0.0～1.0/extdata
function BaseChara:OnHitCharaBullet(blt_type,blt_chr_no,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
	return self.blt[blt_type+1]:OnHitChara(self.id,blt_type,blt_chr_no,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
end
function BaseBullet:OnHitChara(scr_id,blt_type,blt_chr_no,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
	C_RemoveBullet(blt_no,2) -- obj_no,rm_type(0:normal/1:out/2:bomb)
	C_BombObject(scr_id,blt_type,blt_chr_no,blt_no,hx,hy)
	return 1
end

-- 弾が弾に当たった：弾作ったキャラObjNo,hit_obj_no,obj_no,スクリプト番号,弾タイプ,当たった位置x,y/当たった箇所x,y/移動値x,y/残り移動時間0.0～1.0/extdata
function BaseChara:OnHitBulletBullet(blt_type,blt_chr_no,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
	return self.blt[blt_type+1]:OnHitBullet(self.id,blt_type,blt_chr_no,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
end
function BaseBullet:OnHitBullet(scr_id,blt_type,blt_chr_no,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
	return 0
end

-- キャラが爆発範囲内だった：弾作ったキャラObjNo,chr_no,obj_no,スクリプト番号,弾タイプ,当たった弾の位置x,y/爆発位置x,y/範囲の近さによる威力値/extdata
function BaseChara:OnHitCharaBulletBomb(blt_type,hit_chr_no,blt_chr_no,blt_no,hx,hy,power)
	self.blt[blt_type+1]:OnHitCharaBulletBomb(hit_chr_no,blt_chr_no,blt_no,hx,hy,power)
end
function BaseBullet:OnHitCharaBulletBomb(hit_chr_no,blt_chr_no,blt_no,hx,hy,power)
	C_DamageCharaHP(blt_chr_no,hit_chr_no,math.ceil(-self.atk*power))	-- 弾作ったキャラObjNo,HP減らすキャラNo,減らす量
--	C_UpdateCharaStatus(hit_chr_no,-self.atk*power,0,0)	-- chr_no,HP増減値,移動値増減値,ディレイ増減値
end

-- 弾のフレーム処理
function BaseChara:OnFrameBullet(blt_type,blt_no,frame,px,py,vx,vy,ex1,ex2)
	self.blt[blt_type+1]:OnFrame(blt_type,blt_no,frame,px,py,vx,vy,ex1,ex2)
end
function BaseBullet:OnFrame(blt_type,blt_no,frame,px,py,vx,vy,ex1,ex2)
	return
end

-- 弾の軌道処理
function BaseChara:OnGetEphemerisBullet(blt_type,blt_no,frame,px,py,vx,vy,wind,ex1,ex2)
	return self.blt[blt_type+1]:OnGetEphemeris(blt_type,blt_no,frame,px,py,vx,vy,wind,ex1,ex2)
end
function BaseBullet:OnGetEphemeris(blt_type,blt_no,frame,px,py,vx,vy,wind,ex1,ex2)
	return px,py,true
end

function BaseChara:GetFace(type)
	if type == CHARA_FACE_FINE then
		return self.tex_face_fine_x,self.tex_face_fine_y,self.tex_face_w,self.tex_face_h
	elseif type == CHARA_FACE_NORMAL then
		return self.tex_face_normal_x,self.tex_face_normal_y,self.tex_face_w,self.tex_face_h
	end
	return self.tex_face_hurt_x,self.tex_face_hurt_y,self.tex_face_w,self.tex_face_h
end

function BaseChara:GetResultMessage(type)
	if type == CHARA_FACE_FINE then
		return self.tex_face_fine_msg
	elseif type == CHARA_FACE_NORMAL then
		return self.tex_face_normal_msg
	end
	return self.tex_face_hurt_msg
end

-- SEのファイル数
function BaseChara:GetSEFilesCount()
	return #self.se
end

function BaseChara:GetSEFile(index)
	return self.se[index+1]
end

-- 弾の描画
--	vec_angle 飛んでる方向(角度)
--	state=DEF_STATE_MAIN_WAIT/DEF_STATE_MAIN_ACTIVE
function BaseChara:OnDrawBullet(blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
	self.blt[blt_type+1]:OnDraw(self.id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
end
function BaseBullet:OnDraw(scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
	C_UpdateBulletAngle(blt_no,vec_angle)
end
-- 弾のタイプ変更時のイベント
function BaseChara:OnUpdateTypeCharaBullet(blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,type)
	self.blt[blt_type+1]:OnUpdateType(blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,type)
end
function BaseBullet:OnUpdateType(blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,type)
	return
end
-- 弾の状態変更時のイベント
function BaseChara:OnUpdateStateCharaBullet(blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,state)
	self.blt[blt_type+1]:OnUpdateState(blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,state)
end
function BaseBullet:OnUpdateState(blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,state)
	return
end
-- 風向き変更イベント
function BaseChara:OnChangeWindCharaBullet(blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,wind)
	self.blt[blt_type+1]:OnChangeWind(blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,wind)
end
function BaseBullet:OnChangeWind(blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,wind)
	return
end
-- 弾のターンスタート時のイベント
function BaseChara:OnTurnStartBullet(blt_type,blt_chr_no,blt_no,turn,ex1,ex2,act_obj_no)
	self.blt[blt_type+1]:OnTurnStart(blt_type,blt_chr_no,blt_no,turn,ex1,ex2,act_obj_no)
end
function BaseBullet:OnTurnStart(blt_type,blt_chr_no,blt_no,turn,ex1,ex2,act_obj_no)
	return
end
-- 弾のターンエンド時のイベント
function BaseChara:OnTurnEndBullet(blt_type,blt_chr_no,blt_no,turn,ex1,ex2,act_obj_no)
	self.blt[blt_type+1]:OnTurnEnd(blt_type,blt_chr_no,blt_no,turn,ex1,ex2,act_obj_no)
end
function BaseBullet:OnTurnEnd(blt_type,blt_chr_no,blt_no,turn,ex1,ex2,act_obj_no)
	return
end

-- 弾の爆発時のイベント(既に弾が無い場合blt_no=-1,vx,vy,ex,ex=nil)
function BaseChara:OnBombCharaBullet(blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
	self.blt[blt_type+1]:OnBomb(blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
end
function BaseBullet:OnBomb(blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
	
end
-- spellcard ####################
-- スペルカードの情報
function BaseChara:GetSCBaseInfo()
	return self.sc:GetInfo()
end
-- 名前
function BaseChara:GetSCPartInfo()
	return self.sc:GetSCInfo()
end
function BaseBullet:GetSCInfo()
	return self.name,self.exp,self.exp_max
end

-- スペルのフレーム処理
function BaseChara:OnFrameSpell(blt_no,frame,px,py,vx,vy,ex1,ex2)
	self.sc:OnFrame(DEF_BLT_TYPE_SPELL,blt_no,frame,px,py,vx,vy,ex1,ex2)
end

-- 弾の軌道処理
function BaseChara:OnGetEphemerisSpell(blt_no,frame,px,py,vx,vy,wind,ex1,ex2)
	return self.sc:OnGetEphemeris(blt_type,blt_no,frame,px,py,vx,vy,wind,ex1,ex2)
end

-- スペルがキャラに当たった
function BaseChara:OnHitCharaSpell(blt_chr_no,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
	return self.sc:OnHitChara(self.id,DEF_BLT_TYPE_SPELL,blt_chr_no,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
end

-- スペルが弾に当たった：弾作ったキャラObjNo,hit_obj_no,obj_no,スクリプト番号,弾タイプ,当たった位置x,y/当たった箇所x,y/移動値x,y/残り移動時間0.0～1.0/extdata
function BaseChara:OnHitBulletSpell(blt_chr_no,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
	return self.sc:OnHitBullet(self.id,DEF_BLT_TYPE_SPELL,blt_chr_no,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
end

-- 地面に当たった：弾作ったキャラObjNo,obj_no,スクリプト番号,当たった位置x,y/移動値x,y/残り移動時間0.0～1.0/extdata
function BaseChara:OnHitStageSpell(blt_chr_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
	return self.sc:OnHitStage(self.id,DEF_BLT_TYPE_SPELL,blt_chr_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
end

-- 弾のタイプ変更時のイベント
function BaseChara:OnUpdateTypeCharaSpell(blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,type)
	self.sc:OnUpdateType(DEF_BLT_TYPE_SPELL,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,type)
end

-- 弾の状態変更時のイベント
function BaseChara:OnUpdateStateCharaSpell(blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,state)
	self.sc:OnUpdateState(DEF_BLT_TYPE_SPELL,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,state)
end

-- 風向き変更イベント
function BaseChara:OnChangeWindCharaSpell(blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,wind)
	self.sc:OnChangeWind(DEF_BLT_TYPE_SPELL,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,wind)
end

-- 弾のターンエンド時のイベント
function BaseChara:OnTurnEndSpell(blt_chr_no,blt_no,turn,ex1,ex2,act_obj_no)
	self.sc:OnTurnEnd(DEF_BLT_TYPE_SPELL,blt_chr_no,blt_no,turn,ex1,ex2,act_obj_no)
end

-- 弾のターンスタート時のイベント
function BaseChara:OnTurnStartSpell(blt_chr_no,blt_no,turn,ex1,ex2,act_obj_no)
	self.sc:OnTurnStart(DEF_BLT_TYPE_SPELL,blt_chr_no,blt_no,turn,ex1,ex2,act_obj_no)
end

-- 弾の爆発時のイベント(既に弾が無い場合blt_no=-1,vx,vy,ex,ex=nil)
function BaseChara:OnBombCharaSpell(blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
	self.sc:OnBomb(DEF_BLT_TYPE_SPELL,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
end

-- キャラが爆発範囲内だった：弾作ったキャラObjNo,chr_no,obj_no,スクリプト番号,弾タイプ,当たった弾の位置x,y/爆発位置x,y/範囲の近さによる威力値/extdata
function BaseChara:OnHitCharaSpellBomb(hit_chr_no,blt_chr_no,blt_no,hx,hy,power)
	self.sc:OnHitCharaBulletBomb(hit_chr_no,blt_chr_no,blt_no,hx,hy,power)
end

--	vec_angle 飛んでる方向(角度)
--	state=DEF_STATE_MAIN_WAIT/DEF_STATE_MAIN_ACTIVE
function BaseChara:OnDrawSpell(blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
	self.sc:OnDraw(self.id,DEF_BLT_TYPE_SPELL,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
end

-- chara ########################
function BaseChara:GetID()
	return self.id, self.send
end

function BaseChara:GetName()
	return self.name
end

function BaseChara:GetTexFile()
	return self.tex_chr
end

function BaseChara:GetDrawSize()
	return self.draw_w, self.draw_h
end

function BaseChara:GetTexChr()
	return self.tex_chr_x, self.tex_chr_y, self.tex_chr_w, self.tex_chr_h, self.tex_chr_num
end

function BaseChara:GetTexTrg()
	return self.tex_trg_x, self.tex_trg_y, self.tex_trg_w, self.tex_trg_h, self.tex_trg_num
end

function BaseChara:GetTexFace()
	return self.tex_gui_face_x, self.tex_gui_face_y
end

function BaseChara:GetAngleRange()
	return self.angle_range_min,self.angle_range_max,self.shot_h
end

function BaseChara:GetDelay()
	return self.delay
end

function BaseChara:GetMove()
	return self.move
end

-- HP最大値
function BaseChara:GetHP()
	return self.max_hp
end

-- 弾の種類
function BaseChara:GetBltTypeCount()
	return self.blt_type_count,self.blt_sel_count
end
-- アイコンの情報
function BaseChara:GetIcon()
	return self.tex_chr_x, self.tex_chr_y ,self.tex_chr_w, self.tex_chr_h
end

-- 弾発射演出 return trueで終了
function BaseChara:OnTriggerFrame(type,px,py,angle,frame)
	if frame >= 25 then
		return true
	end
	return false
end

-- ターン終了時のイベント
function BaseChara:OnTurnEndChara(chr_no,turn,ex1,ex2)

end

-- ターン開始時のイベント
function BaseChara:OnTurnStartChara(chr_no,turn,ex1,ex2)

end

-- キャラのHPが変更される：キャラのObjNo,max hp, current hp,変更されるHP/extdata
function BaseChara:OnChangeCharaHP(chr_no,max_hp,current_hp,new_hp,ex1,ex2)
	return new_hp		-- 変更されるHPとは違う値にしたい場合、returnする値を変える
end

-- 自分が死んだ時のイベント(DEF_CHARA_DEAD_KILL/DEF_CHARA_DEAD_DROP/DEF_CHARA_DEAD_CLOSE)
-- prv_stt 直前のキャラ状態
function BaseChara:OnDeadChara(chr_no,type,prv_stt)
--	if prv_stt ~= DEF_STATE_DEAD then
		if type ~= DEF_CHARA_DEAD_DROP then
			chr = C_GetCharacterFromObjNo(chr_no)
			if chr~=nil then
				if chr.angle <= 90 or chr.angle >= 270 then			-- 左を向いている場合
					C_SetCharaTexture(chr_no,64,64,96,96)
				else
					C_SetCharaTexture(chr_no,32,64,64,96)
				end
			end
		end
--	end
end
-- キャラがロードされた時のイベント
function BaseChara:OnLoadChara(chr_no,px,py)

end

-- キャラ描画イベント
function BaseChara:OnDrawChara(chr_no,state,angle,vx,vy,frame)
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
	elseif state==DEF_STATE_TRIGGER_BULLET or state==DEF_STATE_TRIGGER_SPELL then
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
	end
end

