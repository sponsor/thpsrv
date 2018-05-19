module("data.scr.chr.004_suwako",package.seeall)

-- chara
Chara = {}

function Chara.new()
	local CharaID = 4
	local bc = require("data.scr.BaseChara")
	local blt1  = {
		id = CharaID,
		hit_range = 10,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 39,
		atk = 200,
		delay = 130,
		icon_x = 96,
		icon_y = 64,
		tex_x = 96,
		tex_y = 64,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/004_suwako/suwako_b10.wav",
				"data/scr/chr/004_suwako/suwako_b11.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			e=math.random(8,16)
			for i=0,e do
				effect_no = C_AddEffect(self.id,64,96,128,160,bx,by,40)
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
			C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,0,0)
			C_PlaySoundSE(self.se[1],0,0)
			return true
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			if vx > 0 then
				C_UpdateBulletAngle(blt_no,frame*20)
			else
				C_UpdateBulletAngle(blt_no,frame*20)
			end
			if frame % 2 then
				effect_no = C_AddEffect(scr_id,0,96,64,160,px,py,20)
				if effect_no ~= -1 then
					C_SetEffectFade(effect_no,-25)
					C_SetEffectScale(effect_no,0.5,0.5)
					C_SetEffectScalling(effect_no,0.04,0.04)
					C_SetEffectRotation(effect_no, 40)
				end
			end
		end,
		OnHitStage = function(self,scr_id,blt_type,blt_chr_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
			if ex1==0 then
				vang = C_GetAngle(vx,vy)--進行角度
				n = C_GetAngle(px-hx,py-hy)	--壁と中心位置の角度
				rang = (vang+180)%360
				rrad = math.rad(n+180-(math.abs(vang)-math.abs(n)))

				arad = math.rad(vang)	--進行角度
				avx = vx/math.cos(arad)	--移動値xy
				avy = vy/math.sin(arad)
		
				C_SetBulletExtData1(blt_no, 1)
				C_UpdateBulletPosition(blt_no,px,py,math.ceil(avx*math.cos(rrad)+0.5),math.ceil(avy*math.sin(rrad)+0.5),0,self.add_vec_y)
				C_BombObject(scr_id,blt_type,blt_chr_no,blt_no,px,py)
			else
				if C_RemoveBullet(blt_no,2) == true then -- obj_no,rm_type(0:normal/1:out/2:bomb)
					C_BombObject(scr_id,blt_type,blt_chr_no,blt_no,hx,hy)
				end
			end
			return 1
		end,
		OnHitChara = function(self,scr_id,blt_type,blt_chr_no,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
			if ex1==0 then
				arad = math.rad(C_GetAngle(vx,vy))	--進行角度
				avx = vx/math.cos(arad)	--移動値xy
				avy = vy/math.sin(arad)
				hrad = math.rad(C_GetAngle(px-hx,py-hy))
				C_SetBulletExtData1(blt_no, 1)
				C_BombObject(scr_id,blt_type,blt_chr_no,blt_no,hx,hy)
				C_UpdateBulletPosition(blt_no,px,py,avx*math.cos(hrad),avy*math.sin(hrad),0,self.add_vec_y)
			else
				if C_RemoveBullet(blt_no,2) == true then -- obj_no,rm_type(0:normal/1:out/2:bomb)
					C_BombObject(scr_id,blt_type,blt_chr_no,blt_no,hx,hy)
				end
			end
			return 1
		end,
	}
	setmetatable(blt1,{ __index = bc.BaseBullet.new()})
	local blt2 = {
		id = CharaID,
		hit_range = 8,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 31,
		atk = 145,
		delay = 165,
		icon_x = 128,
		icon_y = 64,
		tex_x = 128,
		tex_y = 64,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/004_suwako/suwako_b20.wav",
				"data/scr/chr/004_suwako/suwako_b21.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			e=math.random(8,20)
			for i=0,e do
				effect_no = C_AddEffect(self.id,128,96,160,128,bx,by,40)
				if effect_no ~= -1 then
					rnd = math.random(25,100)*0.01
					evx = math.random(0,16)-8
					evy = -math.random(2,8)
					rnd = math.random(15,40)*0.025
					C_SetEffectScale(effect_no,rnd,rnd)
					C_SetEffectFade(effect_no,-math.random(7,16))
					C_SetEffectVector(effect_no, evx,evy,0.0,0.3)
				end
			end
		end,
		Shot = function(self,chr_obj_no,chr_id,blt_type,px,py,vx,vy,vec_angle,power,frame)
			if frame == 0 then
				C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,1,1)
				C_PlaySoundSE(self.se[1],0,0)
				return false
			elseif frame >= 2 then
				chr = C_GetCharacterFromObjNo(chr_obj_no)	-- キャラのテーブル取得
				if tonumber(chr.entity) == 1 then
					ang = tonumber(chr.angle)+90				-- キャラと地面の法線
					rad = math.rad((ang-vec_angle)*2+vec_angle)	-- 発射角と法線から反対側のラジアン計算
					evx = math.cos(rad)*power	-- ラジアンからx,y
					evy = math.sin(rad)*power
					C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,evx,evy,self.add_vec_x,self.add_vec_y,self.hit_range,1,1)
					C_PlaySoundSE(self.se[1],0,0)
				end
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
			if frame % 2 then
				effect_no = C_AddEffect(scr_id,128,96,160,128,px,py,20)
				if effect_no ~= -1 then
					C_SetEffectFade(effect_no,-25)
					C_SetEffectRotate(effect_no, vec_angle)
					C_SetEffectScalling(effect_no, -0.025, -0.025)
				end
			end
		end,
	}
	setmetatable(blt2,{ __index = bc.BaseBullet.new()})

	local blt0 = {
		id = CharaID,
		hit_range = 28,
		add_vec_x = 0,
		add_vec_y = 0,
		bomb_range= 0,
		atk = 100,
		delay = 0,
		icon_x = 160,
		icon_y = 64,
		tex_x = 160,
		tex_y = 96,
		tex_w = 32,
		tex_h = 32,
		bitmsk = {0x0000000f,0x000000f0,0x00000f00,0x0000f000,
				  0x000f0000,0x00f00000,0x0f000000,0xf0000000,},
		bitdel = {0xfffffff0,0xffffff0f,0xfffff0ff,0xffff0fff,
				  0xfff0ffff,0xff0fffff,0xf0ffffff,0x0fffffff,},
		se = {	"data/scr/chr/004_suwako/suwako_b30.wav",
				"data/scr/chr/004_suwako/suwako_b31.wav"},
		Shot = function(self,chr_obj_no,chr_id,blt_type,px,py,vx,vy,vec_angle,power,frame)
			wind = C_GetWindValue()
			blt_no = C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,OBJ_TYPE_GAS,px,py,wind*10,0,-wind,0,self.hit_range,0,0)
			if blt_no ~= -1 then
				C_UpdateBulletState(blt_no,DEF_STATE_WAIT)
			end
			return true
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			C_UpdateBulletAngle(blt_no,frame)
			rnd=math.random(1,2)
			if rnd==1 then
				if (rnd%2) == 1 then
					effect_no = C_AddEffect(scr_id,192,96,256,160,px,py,25)
				else
					effect_no = C_AddEffect(scr_id,192,160,256,224,px,py,25)
				end
				if effect_no ~= -1 then
					rnd=math.random(0,12345)
					C_SetEffectAlpha(effect_no,rnd%50+200)
					C_SetEffectFade(effect_no,-(rnd%10+15))
					C_SetEffectFadeInOut(effect_no,10)
					C_SetEffectRotate(effect_no, rnd%360)
					tmp=(rnd%60+80)*0.01
					C_SetEffectScale(effect_no,tmp,tmp)
					if (rnd%2) == 1 then
						tmp=(rnd%15+10)*0.004
					else
						tmp= (rnd%15+10)*-0.004
					end
					C_SetEffectScalling(effect_no, tmp, tmp)
					tmp=(rnd%90+10)*0.0001
					C_SetEffectVector(effect_no, vx*tmp,vy*tmp,(math.random()-0.5)*0.15,(math.random()-0.5)*0.15)
				end
			end
		end,
		OnHitStage = function(self,scr_id,blt_type,blt_chr_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
			return 0
		end,
		OnHitChara = function(self,scr_id,blt_type,blt_chr_no,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
			sft = hit_obj_no*4
			index = hit_obj_no+1
			dat = bit.band(self.bitmsk[index],ex2)
			hc = bit.rshift(dat,sft)	-- 取得
			ex2 = ex2-dat -- ビット範囲をクリア
			hc = (hc+1) % 10 -- カウントアップ
			if hc == 0 then
				C_DamageCharaHP(blt_chr_no,hit_obj_no,-5)	-- 弾作ったキャラObjNo,HP減らすキャラNo,減らす量
				total = bit.band(ex1,0x0000ffff)
				if (total >= 295) then -- ダメージ上限
					C_RemoveBullet(blt_no,0)
					return 1
				else
					C_SetBulletExtData1(blt_no, total+5+bit.band(ex1,0xffff0000))
				end
			end
			C_SetBulletExtData2(blt_no, ex2 + bit.lshift(hc,sft))
			return 0
		end,
		OnChangeWind = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,wind)
			wind = C_GetWindValue()
			C_UpdateBulletVector(blt_no,wind*10,0,-wind,0)
		end,
		OnTurnStart = function(self,blt_type,blt_chr_no,blt_no,turn,ex1,ex2,act_obj_no)
			wind = C_GetWindValue()
			C_UpdateBulletVector(blt_no,wind*10,0,-wind,0)
		end,
		OnTurnEnd = function(self,blt_type,blt_chr_no,blt_no,turn,ex1,ex2,act_obj_no)
			t = bit.rshift(bit.band(ex1,0xffff0000),16)
			if t>=6 then
				C_RemoveBullet(blt_no,0) -- obj_no,rm_type(0:normal/1:out/2:bomb)
			else
				C_SetBulletExtData1(blt_no, bit.lshift(t+1,16)+bit.band(ex1,0x0000ffff))
			end
		end,
	}
	setmetatable(blt0,{ __index = bc.BaseBullet.new()})
	
	local blt3 = {
		id = CharaID,
		hit_range = 10,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 30,
		atk = 100,
		delay = 125,
		icon_x = 160,
		icon_y = 64,
		tex_x = 160,
		tex_y = 96,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/004_suwako/suwako_b30.wav",
				"data/scr/chr/004_suwako/suwako_b31.wav"},
		Shot = function(self,chr_obj_no,chr_id,blt_type,px,py,vx,vy,vec_angle,power,frame)
			C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,0,0)
			C_PlaySoundSE(self.se[1],0,0)
			return true
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			C_UpdateBulletAngle(blt_no,frame)
			rnd=math.random(1,2)
			if rnd==1 then
				if (rnd%2) == 1 then
					effect_no = C_AddEffect(scr_id,192,96,256,160,px,py,25)
				else
					effect_no = C_AddEffect(scr_id,192,160,256,224,px,py,25)
				end
				if effect_no ~= -1 then
					rnd=math.random(0,12345)
					C_SetEffectAlpha(effect_no,rnd%50+200)
					C_SetEffectFade(effect_no,-(rnd%10+15))
					C_SetEffectFadeInOut(effect_no,10)
					C_SetEffectRotate(effect_no, rnd%360)
					tmp=(rnd%60+80)*0.01
					C_SetEffectScale(effect_no,tmp,tmp)
					if (rnd%2) == 1 then
						tmp=(rnd%15+10)*0.004
					else
						tmp= (rnd%15+10)*-0.004
					end
					C_SetEffectScalling(effect_no, tmp, tmp)
					tmp=(rnd%90+10)*0.0001
					C_SetEffectVector(effect_no, vx*tmp,vy*tmp,(math.random()-0.5)*0.15,(math.random()-0.5)*0.15)
				end
			end
		end,
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			e=math.random(8,20)
			for i=0,e do
				rnd=math.random(1,12345)
				if (rnd%2) == 1 then
					effect_no = C_AddEffect(self.id,192,96,256,160,px,py,45)
				else
					effect_no = C_AddEffect(self.id,192,160,256,224,px,py,45)
				end
				if effect_no ~= -1 then
					rad = math.rad(rnd%360)
					evx = math.cos(rad)
					evy = math.sin(rad)
					tmp = (rnd%60+80)*0.01
					C_SetEffectScale(effect_no,tmp,tmp)
					tmp=(rnd%15+10)*0.001
					C_SetEffectScalling(effect_no, tmp, tmp)
					C_SetEffectFade(effect_no,-(rnd%6+10))
					evx=(math.random()*4-2)
					evy=(math.random()*4-2)
					eax=(math.random()*10-5)*0.01
					eay=(math.random()*10-5)*0.01
					C_SetEffectVector(effect_no, evx,evy,eax,eay)
				end
			end
		end,
		OnHitStage = function(self,scr_id,blt_type,blt_chr_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
			if C_RemoveBullet(blt_no,0) == true then
				C_BombObject(scr_id,blt_type,blt_chr_no,blt_no,px,py)
				blt0:Shot(blt_chr_no,self.id,3,px,py,0,0,0,0,0)
				return 1
			end
			return 0
		end,
		OnHitChara = function(self,scr_id,blt_type,blt_chr_no,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
			if C_RemoveBullet(blt_no,0) == true then
				C_BombObject(scr_id,blt_type,blt_chr_no,blt_no,px,py)
				blt0:Shot(blt_chr_no,self.id,3,px,py,0,0,0,0,0)
				return 1
			end
			return 0
		end,
	}
	setmetatable(blt3,{ __index = bc.BaseBullet.new()})

	local spellcard = {
		id = CharaID,
		name = "土着神「宝永四年の赤蛙」",
		exp = 0,
		exp_max = 750,
		hit_range = 8,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 0,
		atk = 0,
		delay = 650,
		icon_x = 192,
		icon_y = 64,
		tex_x = 128,
		tex_y = 128,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/004_suwako/suwako_s00.wav",
				"data/scr/chr/004_suwako/suwako_s01.wav"},--,
		Spell = function(self,chr_obj_no,chr_id,px,py,vx,vy,vec_angle,power,frame)
			C_CreateBullet(BLT_PROC_TYPE_SCR_SPELL,chr_obj_no,chr_id,DEF_BLT_TYPE_SPELL,OBJ_TYPE_SOLID,px,py,vx,vy,0,self.add_vec_y,self.hit_range,0,0)
			C_PlaySoundSE(self.se[1],0,0)
			return true
		end,
		OnHitCharaBulletBomb = function(self,hit_chr_no,blt_chr_no,blt_no,hx,hy,power)
			return 0
		end,
		OnHitChara = function(self,scr_id,blt_type,blt_chr_no,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
			return 0 -- このイベント時点で弾のベクトルを変える場合は1を返す
		end,
		OnHitStage = function(self,scr_id,blt_type,blt_chr_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
			if C_RemoveBullet(blt_no,0) == true then -- obj_no,rm_type(0:normal/1:out/2:bomb)
				C_BombObject(scr_id,blt_type,blt_chr_no,blt_no,hx,hy)
				tx=0			-- 右向き
				if vx<0 then	-- 左向き
					tx=117
				end
				C_PasteTextureOnStage(scr_id,hx-60,hy-100,0+tx,360,117+tx,474)
				C_UpdateCharaPos(blt_chr_no,hx,hy-107)
			end
			return 1 -- このイベント時点で弾のベクトルを変える場合は1を返す
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			if vx < 0 and ex1 ~= 1 then	-- 左向き
				C_SetBulletTextureIndex(blt_no,1)
				C_SetBulletExtData1(blt_no,1)
			elseif vx>=0 and ex1 == 1 then	-- 右向き
				C_SetBulletTextureIndex(blt_no,0)
				C_SetBulletExtData1(blt_no,0)
			end
			C_UpdateBulletAngle(blt_no,vec_angle)
		end,
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			for i=0,16 do
				ex=(math.random(0,80)-40)
				ey=(math.random(0,80)-40)
				rnd=math.random(1,12345)
				effect_no = C_AddEffect(self.id,192,224,256,288,bx+ex,by+ey,45)
				if effect_no ~= -1 then
					rad = math.rad(rnd%360)
					evx = math.cos(rad)
					evy = math.sin(rad)
					tmp = (rnd%70+80)*0.01
					C_SetEffectScale(effect_no,tmp,tmp)
					tmp=(rnd%15+10)*0.001
					C_SetEffectScalling(effect_no, tmp, tmp)
					C_SetEffectFade(effect_no,-(rnd%6+10))
					ex=(math.random()*6-3)
					ey=(math.random()*6-3)
					eax=(math.random()*10-5)*0.02
					eay=(math.random()*10-5)*0.02
					C_SetEffectVector(effect_no, ex,ey,eax,eay)
				end
			end
			
		end,
	}
	setmetatable(spellcard,{ __index = bc.BaseBullet.new()})
	
	self = {
		id = CharaID,
		name = "洩矢諏訪子",
		tex_chr = "data/scr/chr/004_suwako/suwako.png",
		angle_range_min = 10,
		angle_range_max = 90,
		move = 60,
		delay = 530,
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
		tex_face_hurt_x = 0,
		tex_face_hurt_y = 256,
		tex_face_fine_msg = "神遊びはやっぱりこうでなくちゃね！",
		tex_face_normal_msg = "う～ん…。まだまだ遊び足りないなぁ…。",
		tex_face_hurt_msg = "あーうー…。今日は調子が悪かったんだよ！",
		blt = {blt1, blt2, blt3, blt0},
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
					bgx = 64
					scalex = -1.5
					sallingx = -0.09
					vecx = 3.2
					if angle <= 90 or angle >= 270 then	-- 左を向いている場合
						bgx = 736
						scalex = 1.5
						sallingx = 0.09
						vecx = -3.2
					end
					effect_no = C_AddBGEffect(self.id,256,0,512,356,bgx,256,40)
					if effect_no ~= -1 then
						C_SetBGEffectAlpha(effect_no,0)
						C_SetBGEffectFade(effect_no,10)
						C_SetBGEffectScale(effect_no,scalex, 1.5)
						C_SetBGEffectVector(effect_no,vecx,0,0,0)
					end
				elseif frame == 15 then
					C_PlaySoundSE(self.se[1],0,0)
					effect_no = C_AddEffect(self.id,192,288,256,352,px,py,25)
					if effect_no ~= -1 then
						C_SetEffectFade(effect_no,-10)
						C_SetEffectScale(effect_no,2,2)
						C_SetEffectScalling(effect_no,-0.1,-0.1)
					end
				elseif frame > 15 then
					vs = (math.random()*6+2)
					rad = math.rad(math.random(0,359))
					evx = math.cos(rad)
					evy = math.sin(rad)
					effect_no = C_AddEffect(self.id,224,64,256,96,px+vs*evx*25,py+vs*evy*25,20)
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

