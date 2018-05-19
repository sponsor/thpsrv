module("data.scr.chr.006_yuyuko",package.seeall)

-- chara
Chara = {}

function Chara.new()
	local CharaID = 6
	local bc = require("data.scr.BaseChara")

	local blt0 = {
		id = CharaID,
		hit_range = 10,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 34,
		atk = 250,
		delay = 140,
		icon_x = 96,
		icon_y = 64,
		tex_x = 0,
		tex_y = 384,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/006_yuyuko/yuyuko_b00.wav",
				"data/scr/chr/006_yuyuko/yuyuko_b01.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			e=math.random(8,16)
			for i=0,e do
				effect_no = C_AddEffect(self.id,128,128,160,160,bx,by,40)
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
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			tx = 0
			if vx < 0 then
				tx = 1
			end
			C_SetBulletTextureIndex(blt_no, (tx*3)+(frame/3) % 3)
			C_UpdateBulletAngle(blt_no,0)
		end,
		Shot = function(self,chr_obj_no,chr_id,blt_type,px,py,vx,vy,vec_angle,power,frame)
			if vy <= 0.0 then
				C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,0,0)
			else
				C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,vx,vy,self.add_vec_x,-self.add_vec_y,self.hit_range,0,1)
			end
			C_PlaySoundSE(self.se[1],0,0)
			return true
		end,
		OnFrame = function(self,blt_type,blt_no,frame,px,py,vx,vy,ex1,ex2)
			if ex2 == 1 and py < (-self.bomb_range*2)  then
				C_RemoveBullet(blt_no,1)
			end
		end,
	}
	setmetatable(blt0,{ __index = bc.BaseBullet.new()})
	
	local blt1  = {
		id = CharaID,
		hit_range = 7,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 28,
		atk = 135,
		delay = 155,
		icon_x = 128,
		icon_y = 64,
		tex_x = 0,
		tex_y = 96,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/006_yuyuko/yuyuko_b10.wav",
				"data/scr/chr/006_yuyuko/yuyuko_b11.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			effect_no = C_AddEffect(self.id,192,192,256,256,bx,by,30)
			if effect_no ~= -1 then
				C_SetEffectScale(effect_no,1.5,1.5)
				C_SetEffectFade(effect_no,-10)
			end
		end,
		Shot = function(self,chr_obj_no,chr_id,blt_type,px,py,vx,vy,vec_angle,power,frame)
			ex = 0
			if vx < 0 then
				ex = 1
			end
			avx = math.cos(math.rad(vec_angle+4))*power
			avy = math.sin(math.rad(vec_angle+4))*power
			blt_no = C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,avx,avy,self.add_vec_x,self.add_vec_y,self.hit_range,ex,1)
			if blt_no ~= -1 then
				C_UpdateBulletAngle(blt_no,0)
				C_SetBulletTextureIndex(blt_no, ex*4)
			end
			avx = math.cos(math.rad(vec_angle-4))*power
			avy = math.sin(math.rad(vec_angle-4))*power
			blt_no = C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,avx,avy,self.add_vec_x,self.add_vec_y,self.hit_range,ex,1)
			if blt_no ~= -1 then
				C_UpdateBulletAngle(blt_no,0)
				C_SetBulletTextureIndex(blt_no, ex*4)
			end
			C_PlaySoundSE(self.se[1],0,0)
			return true
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			C_SetBulletTextureIndex(blt_no, (ex1*4)+(frame/4) % 4)
			C_UpdateBulletAngle(blt_no,0)
		end,
	}
	setmetatable(blt1,{ __index = bc.BaseBullet.new()})
	local blt3 = {
		id = CharaID,
		hit_range = 10,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 16,
		atk = 42,
		delay = 180,
		icon_x = 160,
		icon_y = 64,
		tex_x = 0,
		tex_y = 416,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/006_yuyuko/yuyuko_b00.wav",
				"data/scr/chr/006_yuyuko/yuyuko_b01.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			effect_no = C_AddEffect(self.id,192,128,256,192,bx,by,50)
			if effect_no ~= -1 then
				ang = math.random(0,359)
				evx = math.cos(math.rad(ang))
				evy = math.sin(math.rad(ang))
				rnd = math.random(15,30)*0.05
				C_SetEffectScale(effect_no,rnd*0.2*(math.random()*2-0.5),rnd*0.2)
				C_SetEffectScalling(effect_no,math.random()*0.01,math.random()*0.01)
				C_SetEffectRotate(effect_no,math.random(0,359))
				C_SetEffectRotation(effect_no,math.random(8,16)-12)
				C_SetEffectFade(effect_no,-math.random(7,16))
				vs = (math.random()*8+4)
				C_SetEffectVector(effect_no, vs*evx,vs*evy,-evx*rnd*0.15,0.3)
			end
			for i=0,2 do
				effect_no = C_AddEffect(self.id,160,128,192,160,bx,by,40)
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
			avec = self.add_vec_y
			ex=0
			if frame == 0 then
				avec = -self.add_vec_y
				ex=1
			end
			e = math.random(8,10)
			for i=0,e do
				rx = math.random(100,200)-100
				ry = math.random(100,200)-100
				blt_no = C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,vx+rx,vy+ry,self.add_vec_x,avec,self.hit_range,ex,1)
			end
			C_PlaySoundSE(self.se[1],0,0)
			return true
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			tx = 0
			if vx < 0 then
				tx = 1
			end
			C_SetBulletTextureIndex(blt_no, (tx*3)+(frame/3) % 3)
			C_UpdateBulletAngle(blt_no,0)
		end,
		OnFrame = function(self,blt_type,blt_no,frame,px,py,vx,vy,ex1,ex2)
			if ex1 == 1 and py < (-self.bomb_range*2)  then
				C_RemoveBullet(blt_no,1)
			end
		end,
	}
	setmetatable(blt3,{ __index = bc.BaseBullet.new()})

	local blt2 = {
		id = CharaID,
		hit_range = 10,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 43,
		atk = 245,
		delay = 180,
		icon_x = 160,
		icon_y = 64,
		tex_x = 0,
		tex_y = 448,
		tex_w = 64,
		tex_h = 64,
		se = {	"data/scr/chr/006_yuyuko/yuyuko_b20.wav",
				"data/scr/chr/006_yuyuko/yuyuko_b21.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			for i=0,8 do
				effect_no = C_AddEffect(self.id,192,128,256,192,bx,by,50)
				if effect_no ~= -1 then
					ang = math.random(0,359)
					evx = math.cos(math.rad(ang))
					evy = math.sin(math.rad(ang))
					rnd = math.random(15,30)*0.05
					C_SetEffectScale(effect_no,rnd*0.4*(math.random()*2-0.5),rnd*0.4)
					C_SetEffectScalling(effect_no,math.random()*0.01,math.random()*0.01)
					C_SetEffectFade(effect_no,-math.random(7,16))
					C_SetEffectRotate(effect_no,math.random(0,359))
					C_SetEffectRotation(effect_no,math.random(8,16)-12)
					vs = (math.random()*8+4)
					C_SetEffectVector(effect_no, vs*evx,vs*evy,-evx*rnd*0.15,0.3)
				end
			end
			for i=0,12 do
				effect_no = C_AddEffect(self.id,160,128,192,160,bx,by,40)
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
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			tx = 0
			if vx < 0 then
				tx = 1
			end
			C_SetBulletTextureIndex(blt_no, (tx*3)+(frame/3) % 3)
			C_UpdateBulletAngle(blt_no,0)
		end,
		Shot = function(self,chr_obj_no,chr_id,blt_type,px,py,vx,vy,vec_angle,power,frame)
			v = vec_angle % 360
			if vy <= 0.0 then
				C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,v,0)
			else
				C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,vx,vy,self.add_vec_x,-self.add_vec_y,self.hit_range,v,1)
			end
			C_PlaySoundSE(self.se[1],0,0)
			return true
		end,
		OnFrame = function(self,blt_type,blt_no,frame,px,py,vx,vy,ex1,ex2)
			vang = C_GetAngle(vx,vy)%360--進行角度
			if ex2 == 0 then
				if ex1 >= 315 then
					if vang >= 315 then
						return
					end
					ex1 = ex1-360
				end
			else
				if ex1 <= 45 then
					if vang <= 45  then
						return
					end
					ex1 = ex1+360
				end
			end
			if vang <= (ex1+45) and vang >= (ex1-45) then
				return
			end
			C_DbgAddLogNum(ex1)
			C_DbgAddLogNum(vang)
			b = C_GetBulletInfo(blt_no)
			if C_RemoveBullet(blt_no,0) == true then
				blt3:Shot(b.chr_obj_no,self.id,3,b.ax,b.ay,b.vx,b.vy,b.angle,1,ex2)
			end
		end,
	}
	setmetatable(blt2,{ __index = bc.BaseBullet.new()})

	local spellcard = {
		id = CharaID,
		name = "反魂蝶 -八分咲-",
		exp = 250,
		exp_max = 500,
		hit_range = 10,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 50,
		atk = 200,
		delay = 180,
		icon_x = 192,
		icon_y = 64,
		tex_x = 0,
		tex_y = 448,
		tex_w = 64,
		tex_h = 64,
		se = {	"data/scr/chr/006_yuyuko/yuyuko_s00.wav"},
		Spell = function(self,chr_obj_no,chr_id,px,py,vx,vy,vec_angle,power,frame)
			for i=5,45 do
				ang = 180+(i*4)
				fx = px+math.cos(math.rad(ang))*math.random(80,150)
				fy = py+math.sin(math.rad(ang))*math.random(80,150)
				if math.random(2) == 1 then
					effect_no = C_AddEffect(self.id,192,128,256,192,fx,fy,50)
					if effect_no ~= -1 then
						ang = math.random(0,359)
						evx = math.cos(math.rad(ang))
						evy = math.sin(math.rad(ang))
						r = math.random(20,40)*0.02
						C_SetEffectScale(effect_no,r*(math.random()*2-0.5),r)
						C_SetEffectScalling(effect_no,math.random()*0.01,math.random()*0.01)
						C_SetEffectRotate(effect_no,math.random(0,359))
						C_SetEffectRotation(effect_no,math.random(4,12)-8)
						C_SetEffectFade(effect_no,-math.random(5,8))
						vs = (math.random()*8+4)
						C_SetEffectVector(effect_no, vs*evx,vs*evy,-evx*r*0.15,0.3)
					end
				else
					effect_no = C_AddEffect(self.id,160,128,192,160,fx,fy,50)
					if effect_no ~= -1 then
						ang = math.random(0,359)
						evx = math.cos(math.rad(ang))
						evy = math.sin(math.rad(ang))
						rnd = math.random(15,30)*0.1
						C_SetEffectScale(effect_no,rnd,rnd)
						C_SetEffectFade(effect_no,-math.random(7,16))
						vs = (math.random()*8+4)
						C_SetEffectVector(effect_no, vs*evx,vs*evy,-evx*rnd*0.15,0.3)
					end
				end
			end
			pw = power / DEF_MAX_SHOT_POWER
			C_PasteTextureOnStage(self.id,px-128,py-105-(pw*50),256,345,510,448)
			C_PlaySoundSE(self.se[1],0,0)
			return true
		end,
	}
	setmetatable(spellcard,{ __index = bc.BaseBullet.new()})
	
	self = {
		id = CharaID,
		name = "西行寺幽々子",
		tex_chr = "data/scr/chr/006_yuyuko/yuyuko.png",
		angle_range_min = -30,
		angle_range_max = 65,
		move = 70,
		delay = 540,
		max_hp = 860,
		draw_w = 45,
		draw_h = 45,
		shot_h = 12,
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
		tex_face_fine_msg = "運動の後はご飯がおいしいのよね。",
		tex_face_normal_msg = "腹八分みたいで私の性に合わないわね～。",
		tex_face_hurt_x = 0,
		tex_face_hurt_y = 256,
		tex_face_hurt_msg = "今日はおあずけなのかしら…。",
		blt = {blt0, blt1, blt2, blt3},
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
					effect_no = C_AddBGEffect(self.id,256,0,512,335,bgx,320,55)
					if effect_no ~= -1 then
						C_SetBGEffectAlpha(effect_no,0)
						C_SetBGEffectFade(effect_no,5)
						C_SetBGEffectScale(effect_no,2,2)
						C_SetBGEffectVector(effect_no, 0,-2,0,0)
					end
					for i=0,10 do
						effect_no = C_AddEffect(self.id,0,416,32,448,px,py,40)
						if effect_no ~= -1 then
							scl = math.random(75,125)*0.01
							C_SetEffectScale(effect_no,scl,scl)
							C_SetEffectFade(effect_no,-(math.random(5,9)))
							C_SetEffectAnimation(effect_no,4,3,true)
							evx = 1
							if (math.random(2)) == 1 then
								evx = -1
							end
							C_SetEffectVector(effect_no, (math.random(20,60)*0.1)*evx,math.random(4,12)*0.1,-evx*2*0.05,-math.random(5,15)*0.01)
						end
					end
				elseif frame >= 60 then
					return true
				elseif frame > 4 and frame < 60 then
					e=math.random(3)
					if e == 1 then
						bgx = 620
						stgw = C_GetStageWidth()
						if (stgw/2) <= px then	-- ステージの右よりの場合は左側に表示
							bgx = 240
						end
						fx = math.random(200)+bgx-100
						if math.random(2) == 1 then
							evx = 1
							tex = 0
							if math.random(2) == 1 then
								evx = -1
								tex = 1
							end
							fy=math.random(320,500)
							effect_no = C_AddEffect(self.id,tex*192,448,(tex*192)+64,512,fx,fy,50-frame)
							if effect_no ~= -1 then
								C_SetEffectAlpha(effect_no,math.random(180,240))
								scl = math.random(75,125)*0.02
								C_SetEffectScale(effect_no,scl,scl)
								C_SetEffectFadeInOut(effect_no,10)
								C_SetEffectAnimation(effect_no,4,3,true)
								C_SetEffectVector(effect_no, math.random(2,6)*evx,0,-evx*2*0.05,-(math.random(5,15))*0.1)
							end
						else
							fy = math.random(150,450)
							effect_no = C_AddEffect(self.id,192,128,256,192,fx,fy,50-frame)
							if effect_no ~= -1 then
								ang = math.random(0,359)
								evx = math.cos(math.rad(ang))
								evy = math.sin(math.rad(ang))
								r = math.random(15,30)*0.1
								C_SetEffectScale(effect_no,r*0.5*(math.random()*2-0.5),r*0.5)
								C_SetEffectScalling(effect_no,math.random()*0.01,math.random()*0.01)
								C_SetEffectRotate(effect_no,math.random(0,359))
								C_SetEffectRotation(effect_no,math.random(4,12))
								vs = (math.random()*8+4)
								C_SetEffectAlpha(effect_no,math.random(96,128))
								C_SetEffectVector(effect_no, vs*evx,vs*evy,-evx*r*0.15,0.3)
							end
						end
					end
					if e<3 then
						ang = 180+(frame*3)
						fx = px+math.cos(math.rad(ang))*math.random(80,150)
						fy = py+math.sin(math.rad(ang))*math.random(80,150)
						effect_no = C_AddEffect(self.id,192,128,256,192,fx,fy,60-frame)
						if effect_no ~= -1 then
							ang = math.random(0,359)
							evx = math.cos(math.rad(ang))
							evy = math.sin(math.rad(ang))
							r = math.random(20,40)*0.05
							C_SetEffectScale(effect_no,r*(math.random()*2-0.5),r)
							C_SetEffectScalling(effect_no,math.random()*0.01,math.random()*0.01)
							C_SetEffectRotate(effect_no,math.random(0,359))
							C_SetEffectRotation(effect_no,math.random(4,12)-8)
							C_SetEffectAlpha(effect_no,math.random(96,128))
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

