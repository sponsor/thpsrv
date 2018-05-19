module("data.scr.BaseStage",package.seeall)

-- define --
-- bullet
BaseBullet = {}

function BaseBullet.new()
	local self = {
		hit_range = 10,
		bomb_range= 30,
		add_vec_x = 0,
		add_vec_y = 20,
		atk = 100,
		tex_x = 32,
		tex_y = 64,
		tex_w = 32,
		tex_h = 32,
		ephemeris = false,
	}
	return setmetatable(self, { __index = BaseBullet })
end

-- stage
BaseStage = {}

function BaseStage.new()
	local self = {
		id = 0,
		name = "stage1",
		send = true,
		thumnail = "data/img/stg/tBase.png",
		thumnail_w = 128,
		thumnail_h = 128,
		stage = "data/img/stg/sBase.png",
		stage_w = 1024,
		stage_h = 1024,
		bg = "data/img/stg/bBase.png",
		bg_w = 1024,
		bg_h = 1024,
		bgm = "data/bgm/stage00.ogg",
		blt = {},
		se = {}
	}
	self.blt_type_count = #self.blt
	return setmetatable(self, { __index = BaseStage })
end

-- procedure --
-- stage --------------------
-- info
function BaseStage:GetID()
	return self.id, self.send
end

function BaseStage:GetName()
	return self.name
end

function BaseStage:GetThumnailInfo()
	return self.thumnail,self.thumnail_w,self.thumnail_h
end

function BaseStage:GetStageInfo()
	return self.stage,self.stage_w,self.stage_h
end

function BaseStage:GetBGInfo()
	return self.bg,self.bg_w,self.bg_h
end

-- 弾の種類
function BaseStage:GetBltTypeCount()
	return self.blt_type_count
end

-- 弾の情報
function BaseStage:GetBltInfo(num)
	return self.blt[num+1]:GetInfo()
end
function BaseBullet:GetInfo()
	return self.atk,self.tex_x,self.tex_y,self.tex_w,self.tex_h,self.hit_range,self.bomb_range,self.ephemeris
end

-- SEのファイル数
function BaseStage:GetSEFilesCount()
	return #self.se
end

function BaseStage:GetSEFile(index)
	return self.se[index+1]
end

function BaseStage:GetBGMFile()
	return self.bgm
end

-- event
---- ステージ
-- ステージ弾生成時イベント
--function BaseStage:CreateBullet(chr_obj_no,blt_type,px,py,vx,vy)
--	return 0
--end

-- 地面に当たった：弾タイプ,obj_no,スクリプト番号,当たった位置x,y/移動値x,y/残り移動時間0.0～1.0/extdata
function BaseStage:OnHitStageBullet(blt_type,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
	return self.blt[blt_type+1]:OnHitStage(self.id,blt_type,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
end
function BaseBullet:OnHitStage(scr_id,blt_type,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
	C_BombObject(scr_id,blt_type,STAGE_OBJ_NO,blt_no,hx,hy)
	C_RemoveBullet(blt_no,2) -- obj_no,rm_type(0:normal/1:out/2:bomb)
	return 1
end

-- キャラに当たった：弾タイプ,hit_obj_no,obj_no,当たった位置x,y/当たった箇所x,y/移動値x,y/残り移動時間0.0～1.0/extdata
function BaseStage:OnHitCharaBullet(blt_type,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
	return self.blt[blt_type+1]:OnHitChara(self.id,blt_type,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
end
function BaseBullet:OnHitChara(scr_id,blt_type,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
	C_BombObject(scr_id,blt_type,STAGE_OBJ_NO,blt_no,hx,hy)
	C_RemoveBullet(blt_no,2) -- obj_no,rm_type(0:normal/1:out/2:bomb)
	return 1
end

-- 弾が弾に当たった：弾タイプ,hit_obj_no,obj_no,スクリプト番号,当たった位置x,y/当たった箇所x,y/移動値x,y/残り移動時間0.0～1.0/extdata
function BaseStage:OnHitBulletBullet(blt_type,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
	return self.blt[blt_type+1]:OnHitBullet(self.id,blt_type,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
end
function BaseBullet:OnHitBullet(scr_id,blt_type,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
	return 0
end

-- ステージ削除イベント(弾ObjNo,弾タイプ,位置x,y,削れたピクセル数)
function BaseStage:OnEraseStage(blt_type,chr_obj_no,blt_no,px,py,pxls)
	return
end

-- ターンエンド時のイベント アクティブが居ない場合はobj_no=-1
function BaseStage:OnTurnEndStage(turn,act_obj_no,next_obj_no)
--	C_CreateBullet(BLT_PROC_TYPE_SCR_STAGE,STAGE_OBJ_NO,self.id,0,1,100,10,0,0,self.blt[1].add_vec_x,self.blt[1].add_vec_y,self.blt[1].hit_range,1,1)
end

---- 弾
-- ステージ弾が爆発した時、爆発範囲内にキャラが居た
function BaseStage:OnHitCharaStageBulletBomb(blt_type,hit_chr_no,blt_no,hx,hy,power)
	self.blt[blt_type+1]:OnHitCharaStageBulletBomb(hit_chr_no,blt_no,hx,hy,power)
end
function BaseBullet:OnHitCharaStageBulletBomb(hit_chr_no,blt_no,hx,hy,power)
	C_DamageCharaHP(DEF_STAGE_OBJ_NO,hit_chr_no,math.ceil(-self.atk*power))	-- 弾作ったキャラObjNo,HP減らすキャラNo,減らす量
--	C_UpdateCharaStatus(hit_chr_no,-self.atk*power,0,0)	-- chr_no,HP増減値,移動値増減値,ディレイ増減値
end

-- 弾のタイプが変更された時のイベント
function BaseStage:OnUpdateTypeStageBullet(blt_type,blt_no,px,py,vx,vy,ex1,ex2,type)
	self.blt[blt_type+1]:OnUpdateType(blt_type,blt_no,px,py,vx,vy,ex1,ex2,type)
end
function BaseBullet:OnUpdateType(blt_type,blt_no,px,py,vx,vy,ex1,ex2,type)
	return
end

-- 弾の状態が変更された時のイベント
function BaseStage:OnUpdateStateStageBullet(blt_type,blt_no,px,py,vx,vy,ex1,ex2,state)
	self.blt[blt_type+1]:OnUpdateState(blt_type,blt_no,px,py,vx,vy,ex1,ex2,state)
end
function BaseBullet:OnUpdateState(blt_type,blt_no,px,py,vx,vy,ex1,ex2,state)
	return
end

-- 風向き変更イベント
function BaseStage:OnChangeWindStageBullet(blt_type,blt_no,px,py,vx,vy,ex1,ex2,wind)
	self.blt[blt_type+1]:OnChangeWind(blt_type,blt_no,px,py,vx,vy,ex1,ex2,wind)
end
function BaseBullet:OnChangeWind(blt_type,blt_no,px,py,vx,vy,ex1,ex2,wind)
	return
end

-- 弾のターンスタート時のイベント
function BaseStage:OnTurnStartBullet(blt_type,blt_no,turn,ex1,ex2,act_obj_no)
	self.blt[blt_type+1]:OnTurnStart(blt_type,blt_no,turn,ex1,ex2,act_obj_no)
end
function BaseBullet:OnTurnStart(blt_type,blt_no,turn,ex1,ex2,act_obj_no)
	return
end
-- 弾のターンエンド時のイベント
function BaseStage:OnTurnEndBullet(blt_type,blt_no,turn,ex1,ex2,act_obj_no)
	self.blt[blt_type+1]:OnTurnEnd(blt_type,blt_no,turn,ex1,ex2,act_obj_no)
end
function BaseBullet:OnTurnEnd(blt_type,blt_no,turn,ex1,ex2,act_obj_no)
	return
end

-- 弾のフレーム処理
function BaseStage:OnFrameBullet(blt_type,blt_no,frame,px,py,vx,vy,ex1,ex2)
	self.blt[blt_type+1]:OnFrame(blt_type,blt_no,frame,px,py,vx,vy,ex1,ex2)
end
function BaseBullet:OnFrame(blt_type,blt_no,frame,px,py,vx,vy,ex1,ex2)
	return
end

-- 弾の軌道処理
function BaseStage:OnGetEphemerisBullet(blt_type,blt_no,frame,px,py,vx,vy,wind,ex1,ex2)
	return self.blt[blt_type+1]:OnGetEphemeris(blt_type,blt_no,frame,px,py,vx,vy,wind,ex1,ex2)
end
function BaseBullet:OnGetEphemeris(blt_type,blt_no,frame,px,py,vx,vy,wind,ex1,ex2)
	return px,px,true
end

-- 弾の描画
--	vec_angle 飛んでる方向(角度)
--	state=OBJ_STATE_MAIN_WAIT/OBJ_STATE_MAIN_ACTIVE
function BaseStage:OnDrawBullet(blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
	self.blt[blt_type+1]:OnDraw(blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
end
function BaseBullet:OnDraw(blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
	C_UpdateBulletAngle(blt_no,vec_angle)
end

-- 弾の爆発時のイベント(既に弾が無い場合blt_no=-1,vx,vy,ex,ex=nil)
function BaseStage:OnBombStageBullet(blt_type,blt_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
	self.blt[blt_type+1]:OnBomb(blt_type,blt_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
end
function BaseBullet:OnBomb(blt_type,blt_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
	
end

-- ステージがロードされた時のイベント
function BaseStage:OnLoadStage()
	
end