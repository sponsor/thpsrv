module("data.scr.chr.008_alice",package.seeall)

-- ���������Q�L�����ǉ��f�[�^�u�A���X�E�}�[�K�g���C�h�v
-- author:�����̂��ۂ񂳁[
--
-- �y�ӎ��E���C�Z���X�z
-- �J�b�g�C���A�t�F�[�X�͂������l�ɕ`���Ă��������܂����B
-- http://ettamu.jugem.jp/
--
-- �h�b�g�G�́y7B�z�l�̍쐬�����f�ނ���ɍ쐬���܂����B
-- [�_�ˏW��]http://do-t.cool.ne.jp/dots/
--
-- ���ʉ��̓}�b�`���C�J�@�Y�l�̉������g�p�����Ă��������܂����B
--
-- �y�Ĕz�z�ɂ��āz
-- ���̃f�[�^�͓��������Q�̃Q�[���f�[�^�Ƃ��Ă̂ݍĔz�z�\�Ƃ��܂��B

-- chara
Chara = {}

function Chara.new()
	local CharaID = 8
	local bc = require("data.scr.BaseChara")
	local blt1  = {
		id = CharaID,
		hit_range = 5,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 40,
		atk = 205,
		delay = 140,
		icon_x = 96,
		icon_y = 64,
		tex_x = 96,
		tex_y = 64,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/008_alice/alice_b10.wav",
				"data/scr/chr/008_alice/alice_b11.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			effect_no = C_AddEffect(self.id,0,96,64,160,bx,by,30)
			if effect_no ~= -1 then
				C_SetEffectFade(effect_no,-8)
			end
			for i=0,8 do
				effect_no = C_AddEffect(self.id,64,96,96,128,bx,by,40)
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
			for i=0,8 do
				effect_no = C_AddEffect(self.id,192,128,256,192,px,py,45)
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
		Shot = function(self,chr_obj_no,chr_id,blt_type,px,py,vx,vy,vec_angle,power,frame)
			tx = 0
			if vx<0 then
				tx=1
			end
			C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,tx,1)
			C_PlaySoundSE(self.se[1],0,0)
			return true
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			if vx > 0 then
				C_UpdateBulletAngle(blt_no,frame*40)
			else
				C_UpdateBulletAngle(blt_no,frame*40)
			end
		end,
	}
	setmetatable(blt1,{ __index = bc.BaseBullet.new()})
	
	local blt2 = {
		id = CharaID,
		hit_range = 5,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 25,
		atk = 75,
		delay = 140,
		icon_x = 128,
		icon_y = 64,
		tex_x = 224,
		tex_y = 96,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/008_alice/alice_b10.wav",
				"data/scr/chr/008_alice/alice_b11.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			effect_no = C_AddEffect(self.id,0,96,64,160,bx,by,30)
			if effect_no ~= -1 then
				C_SetEffectFade(effect_no,-8)
			end
			for i=0,8 do
				effect_no = C_AddEffect(self.id,64,96,96,128,bx,by,40)
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
			for i=0,8 do
				effect_no = C_AddEffect(self.id,192,128,256,192,px,py,45)
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
		Shot = function(self,chr_obj_no,chr_id,blt_type,px,py,vx,vy,vec_angle,power,frame)
			tx = 0
			if vx<0 then
				tx=1
			end
			if frame == 0 then
				avx = math.cos(math.rad(vec_angle+10))*power
				avy = math.sin(math.rad(vec_angle+10))*power
				C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,avx,avy,self.add_vec_x,self.add_vec_y,self.hit_range,tx,1)
				C_PlaySoundSE(self.se[1],0,0)
			elseif frame == 1 then
				C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,tx,1)
				C_PlaySoundSE(self.se[1],0,0)
			elseif frame == 2 then
				avx = math.cos(math.rad(vec_angle-10))*power
				avy = math.sin(math.rad(vec_angle-10))*power
				C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,avx,avy,self.add_vec_x,self.add_vec_y,self.hit_range,tx,1)
				C_PlaySoundSE(self.se[1],0,0)
				return true
			end
			return false
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			if vx > 0 then
				C_UpdateBulletAngle(blt_no,frame*40)
			else
				C_UpdateBulletAngle(blt_no,frame*40)
			end
		end,
	}
	setmetatable(blt2,{ __index = bc.BaseBullet.new()})
	
	local blt3  = {
		id = CharaID,
		hit_range = 5,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 48,
		atk = 300,
		delay = 160,
		icon_x = 160,
		icon_y = 64,
		tex_x = 160,
		tex_y = 64,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/008_alice/alice_b10.wav",
				"data/scr/chr/008_alice/alice_b31.wav",
				"data/scr/chr/008_alice/alice_b32.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[3],0,0)
			effect_no = C_AddEffect(self.id,0,96,64,160,bx,by,30)
			if effect_no ~= -1 then
				C_SetEffectFade(effect_no,-8)
			end
			for i=0,16 do
				effect_no = C_AddEffect(self.id,64,96,96,128,bx,by,40)
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
			for i=0,8 do
				effect_no = C_AddEffect(self.id,192,128,256,192,px,py,45)
				if effect_no ~= -1 then
					rad = math.rad(rnd%360)
					evx = math.cos(rad)
					evy = math.sin(rad)
					tmp = (rnd%60+80)*0.01
					C_SetEffectScale(effect_no,tmp,tmp)
					tmp=(rnd%15+10)*0.001
					C_SetEffectScalling(effect_no, tmp, tmp)
					C_SetEffectFade(effect_no,-(rnd%6+10))
					evx=(math.random()*5-2)
					evy=(math.random()*5-2)
					eax=(math.random()*10-5)*0.01
					eay=(math.random()*10-5)*0.01
					C_SetEffectVector(effect_no, evx,evy,eax,eay)
				end
			end
		end,
		Shot = function(self,chr_obj_no,chr_id,blt_type,px,py,vx,vy,vec_angle,power,frame)
			C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,2049,0)
			C_PlaySoundSE(self.se[1],0,0)
			return true
		end,
		OnHitChara = function(self,scr_id,blt_type,blt_chr_no,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
			return 0
		end,
		OnHitStage = function(self,scr_id,blt_type,blt_chr_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
			if ex2 == 0 then
				if py < hy then
					C_SetBulletExtData2(blt_no, 2)
					C_UpdateBulletState(blt_no,DEF_STATE_WAIT)
				else
					C_UpdateBulletVector(blt_no,0,0,0,self.add_vec_y)
					C_SetBulletExtData2(blt_no, 1)
					C_SetBulletExtData1(blt_no, py)
				end
			elseif ex2 == 1 then
				if py == ex1 or py < hy then
					C_SetBulletExtData2(blt_no, 2)
					C_UpdateBulletState(blt_no,DEF_STATE_WAIT)
				else
					C_UpdateBulletVector(blt_no,0,0,0,self.add_vec_y)
					C_SetBulletExtData1(blt_no, py)
				end
			end
			return 1
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			if state~=DEF_STATE_WAIT then
				if vx > 0 then
					C_UpdateBulletAngle(blt_no,frame*40)
				else
					C_UpdateBulletAngle(blt_no,frame*40)
				end
			end
		end,
		OnUpdateState = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,state)
			if state==DEF_STATE_WAIT then
				C_UpdateBulletVector(blt_no,0,0,0,self.add_vec_y)
				C_UpdateObjectType(blt_no,OBJ_TYPE_LIQUID)
				C_PlaySoundSE(self.se[2],0,0)
			end
		end,
		OnTurnStart = function(self,blt_type,blt_chr_no,blt_no,turn,ex1,ex2,act_obj_no)
			if act_obj_no == blt_chr_no then
				blt = C_GetBulletInfo(blt_no)
				if C_RemoveBullet(blt_no,0) == true then
					C_BombObject(self.id,blt_type,blt_chr_no,blt_no,blt.ax,blt.ay)
				end
			end
		end,
	}
	setmetatable(blt3,{ __index = bc.BaseBullet.new()})
	
	local spellcard = {
		id = CharaID,
		name = "�푀�u�h�[���Y�E�H�[�v",
		exp = 150,
		exp_max = 550,
		hit_range = 10,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 35,
		atk = 100,
		delay = 180,
		icon_x = 192,
		icon_y = 64,
		tex_x = 160,
		tex_y = 96,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/008_alice/alice_b10.wav",
				"data/scr/chr/008_alice/alice_s01.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			ang = math.random(0,3)*90+45
			effect_no = C_AddEffect(self.id,96,96,160,160,bx,by,15)
			if effect_no ~= -1 then
				scale = math.random(75,125)*0.01
				C_SetEffectScale(effect_no,scale,scale)
				C_SetEffectFade(effect_no,-20)
				C_SetEffectRotate(effect_no,ang)
			end
			ang = math.random(0,60)+ang+60
			effect_no = C_AddEffect(self.id,96,96,160,160,bx,by,15)
			if effect_no ~= -1 then
				scale = math.random(75,125)*0.01
				C_SetEffectScale(effect_no,scale,scale)
				C_SetEffectFade(effect_no,-20)
				C_SetEffectRotate(effect_no,ang)
			end
		
		end,
		OnFrame = function(self,blt_type,blt_no,frame,px,py,vx,vy,ex1,ex2)
			if frame == 15 and ex2==0 then
				wind = C_GetWindValue()
				C_UpdateBulletState(blt_no,DEF_STATE_ACTIVE)
				C_UpdateBulletVector(blt_no,0,0,-wind,0)
				vang = C_GetAngle(vx,vy)--�i�s�p�x
				rang = (vang+180)%360
				C_SetBulletExtData1(blt_no, vang)
			elseif frame == 30 then
				b = C_GetBulletInfo(blt_no)
				r = math.rad(ex1)
				C_BombObject(self.id,blt_type,b.chr_obj_no,blt_no,px+math.cos(r)*45,py+math.sin(r)*45)
			elseif frame == 45 then
				C_RemoveBullet(blt_no,0) -- obj_no,rm_type(0:normal/1:out/2:bomb)
			end
		end,
		Spell = function(self,chr_obj_no,chr_id,px,py,vx,vy,vec_angle,power,frame)
			if frame == 0 then
				avx = math.cos(math.rad(vec_angle+30))* math.max(power,400)
				avy = math.sin(math.rad(vec_angle+30))* math.max(power,400)
				C_CreateBullet(BLT_PROC_TYPE_SCR_SPELL,chr_obj_no,chr_id,DEF_BLT_TYPE_SPELL,OBJ_TYPE_SOLID,px,py,avx,avy,self.add_vec_x,self.add_vec_y,self.hit_range,0,0)
				C_PlaySoundSE(self.se[1],0,0)
			elseif frame == 1 then
				avx = math.cos(math.rad(vec_angle))* math.max(power,400)
				avy = math.sin(math.rad(vec_angle))* math.max(power,400)
				C_CreateBullet(BLT_PROC_TYPE_SCR_SPELL,chr_obj_no,chr_id,DEF_BLT_TYPE_SPELL,OBJ_TYPE_SOLID,px,py,avx,avy,self.add_vec_x,self.add_vec_y,self.hit_range,0,0)
				C_PlaySoundSE(self.se[1],0,0)
			elseif frame == 2 then
				avx = math.cos(math.rad(vec_angle-30))* math.max(power,400)
				avy = math.sin(math.rad(vec_angle-30))* math.max(power,400)
				C_CreateBullet(BLT_PROC_TYPE_SCR_SPELL,chr_obj_no,chr_id,DEF_BLT_TYPE_SPELL,OBJ_TYPE_SOLID,px,py,avx,avy,self.add_vec_x,self.add_vec_y,self.hit_range,0,0)
				C_PlaySoundSE(self.se[1],0,0)
				return true
			end
			return false
		end,
		OnHitChara = function(self,scr_id,blt_type,blt_chr_no,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
			return 0 -- ���̃C�x���g���_�Œe�̃x�N�g����ς���ꍇ��1��Ԃ�
		end,
		OnHitStage = function(self,scr_id,blt_type,blt_chr_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
			if ex2 == 0 then
				C_UpdateBulletState(blt_no,DEF_STATE_ACTIVE)
				vang = C_GetAngle(vx,vy)--�i�s�p�x
				rang = (vang+180)%360
				C_SetBulletExtData1(blt_no, vang)
				wind = C_GetWindValue()
				C_UpdateBulletVector(blt_no,0,0,-wind,0)
				return 1
			end
			return 0 -- ���̃C�x���g���_�Œe�̃x�N�g����ς���ꍇ��1��Ԃ�
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			if ex2 ~= 0 then
				tx = 0
				if ex2 == 1 then
					tx = 1
				end
				C_SetBulletTextureIndex(blt_no, tx)
				C_UpdateBulletAngle(blt_no,0)
			else
				tx = 0
				if vx < 0 then
					tx = 1
				end
				C_SetBulletTextureIndex(blt_no, tx)
				C_UpdateBulletAngle(blt_no,0)
			end
		end,
		OnUpdateState = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,state)
			if state==DEF_STATE_ACTIVE then
				effect_no = C_AddEffect(self.id,160,128,192,160,px,py,30)
				if vx < 0 then
					C_SetBulletExtData2(blt_no, 1)
				else
					C_SetBulletExtData2(blt_no, 2)
				end
				if effect_no ~= -1 then
					C_SetEffectScalling(effect_no,0.1,0.1)
					C_SetEffectFade(effect_no,-9)
				end
			end
		end,
	}
	setmetatable(spellcard,{ __index = bc.BaseBullet.new()})
	
	self = {
		id = CharaID,
		name = "�A���X�E�}�[�K�g���C�h",
		tex_chr = "data/scr/chr/008_alice/alice.png",
		angle_range_min = 15,
		angle_range_max = 95,
		move = 80,
		delay = 535,
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
		tex_face_fine_msg = "�U�߂Ă�萔�͑����ɂ�����B\n���F�̐l�`�̑O�ɋM���͂Ȃɂ��ł���H",
		tex_face_normal_msg = "���ꂪ�A�����I�[�g�œ����l�`�Ȃ�����Ɗy�Ȃ̂�����E�E�H\n����Ƃ��E�E�B",
		tex_face_hurt_x = 0,
		tex_face_hurt_y = 256,
		tex_face_hurt_msg = "�����̐l�`�̃e�X�g�͂���ȂƂ���ˁB\n�t�������Ă���Ă��肪�Ƃ��B",
		blt = {blt1,blt2,blt3},
		sc = spellcard,
		se = {	"data/se/spell00.wav"},
		OnLoadChara = function(self,chr_no,px,py)
			C_SetCharaExtData1(chr_no, 0xFF)
		end,
		OnTriggerFrame = function(self,type,px,py,angle,frame)
			if type == DEF_BLT_TYPE_SPELL then	-- �X�y���J�[�h���o
				if frame == 0 then
					C_HideStage()
				elseif frame == 4 then
					C_PlaySoundSE(self.se[1],0,0)
					bgx = 600
					stgw = C_GetStageWidth()
					if (stgw/2) <= px then	-- �X�e�[�W�̉E���̏ꍇ�͍����ɕ\��
						bgx = 200
					end
					effect_no = C_AddBGEffect(self.id,256,0,512,512,bgx,20,25)
					if effect_no ~= -1 then
						C_SetBGEffectFadeInOut(effect_no,10)
						C_SetBGEffectScale(effect_no,2,2)
						C_SetBGEffectVector(effect_no, 0,12,0,0)
					end
				elseif frame == 15 then
					effect_no = C_AddEffect(self.id,0,416,96,512,px,py,40)
					if effect_no ~= -1 then
						C_SetEffectFade(effect_no,-6)
						C_SetEffectScalling(effect_no,-0.1,-0.1)
						C_SetEffectRotation(effect_no, 40)
					end
				elseif frame >= 60 then
					C_ShowStage()
					return true
				elseif frame == 30 then
					bgx = 780
					stgw = C_GetStageWidth()
					if (stgw/2) <= px then	-- �X�e�[�W�̉E���̏ꍇ�͍����ɕ\��
						bgx = 380
					end
					effect_no = C_AddBGEffect(self.id,192,192,256,320,bgx,48,30)
					if effect_no ~= -1 then
						C_SetBGEffectAlpha(effect_no,64)
						C_SetBGEffectFade(effect_no,10)
						C_SetBGEffectScale(effect_no,2,2)
						C_SetBGEffectVector(effect_no, -1,3,0,0)
					end
				elseif frame == 20 then
					bgx = 600
					stgw = C_GetStageWidth()
					if (stgw/2) <= px then	-- �X�e�[�W�̉E���̏ꍇ�͍����ɕ\��
						bgx = 200
					end
					effect_no = C_AddBGEffect(self.id,256,0,512,320,bgx,100,40)
					if effect_no ~= -1 then
						C_SetBGEffectAlpha(effect_no,64)
						C_SetBGEffectFade(effect_no,10)
						C_SetBGEffectScale(effect_no,2,2)
						C_SetBGEffectVector(effect_no, 0,12,0,-0.25)
					end
				end
			elseif frame >= 25 then
				return true
			end
			return false
		end,
		-- �L�����`��C�x���g
		OnDrawChara = function(self,chr_no,state,angle,vx,vy,frame)
			if state==DEF_STATE_ACTIVE or state==DEF_STATE_WAIT then	-- �A�N�e�B�u���
				if (frame % 5) == 0 then				-- 5frame���Ƃɐ؂�ւ�
					tex_idx = (frame / 5) % self.tex_chr_num	-- ���Ԗڂ̃e�N�X�`����\�������邩�v�Z
					tex_left = self.tex_chr_x					-- �����ʒu��ێ�
					if angle <= 90 or angle >= 270 then			-- ���������Ă���ꍇ
						tex_idx = tex_idx + self.tex_chr_num	-- �e�N�X�`�������C���f�b�N�X�����炷
					end
					tex_left = self.tex_chr_x + self.tex_chr_w * tex_idx	-- �e�N�X�`�����[���v�Z
					-- �e�N�X�`���ݒ�
					C_SetCharaTexture(chr_no,tex_left+1,self.tex_chr_y+1,tex_left+self.tex_chr_w-1,self.tex_chr_y+self.tex_chr_h-1)
				end
			elseif state==DEF_STATE_TRIGGER_BULLET then
				if (frame % 5) == 0 then				-- 5frame���Ƃɐ؂�ւ�
					tex_idx = (frame / 5)
					if tex_idx >= self.tex_trg_num then		-- �C���f�b�N�X�l�̏C��
						tex_idx = self.tex_trg_num -1
					end
					tex_left = self.tex_trg_x					-- �����ʒu��ێ�
					if angle <= 90 or angle >= 270 then			-- ���������Ă���ꍇ
						tex_idx = tex_idx + self.tex_trg_num	-- �e�N�X�`�������C���f�b�N�X�����炷
					end
					tex_left = self.tex_trg_x + self.tex_trg_w * tex_idx	-- �e�N�X�`�����[���v�Z
					-- �e�N�X�`���ݒ�
					C_SetCharaTexture(chr_no,tex_left+1,self.tex_trg_y+1,tex_left+self.tex_trg_w-1,self.tex_trg_y+self.tex_trg_h-1)
				end
			elseif state==DEF_STATE_TRIGGER_SPELL then
				if (frame % 5) == 0 then				-- 5frame���Ƃɐ؂�ւ�
					tex_idx = (frame / 5)
					if tex_idx >= 14 then					-- �C���f�b�N�X�l�̏C��
						tex_idx = self.tex_trg_num -1
					elseif tex_idx < 9 then
						tex_idx = 0
					else
						tex_idx = tex_idx - 9
					end
					tex_left = self.tex_trg_x					-- �����ʒu��ێ�
					if angle <= 90 or angle >= 270 then			-- ���������Ă���ꍇ
						tex_idx = tex_idx + self.tex_trg_num	-- �e�N�X�`�������C���f�b�N�X�����炷
					end
					tex_left = self.tex_trg_x + self.tex_trg_w * tex_idx	-- �e�N�X�`�����[���v�Z
					-- �e�N�X�`���ݒ�
					C_SetCharaTexture(chr_no,tex_left+1,self.tex_trg_y+1,tex_left+self.tex_trg_w-1,self.tex_trg_y+self.tex_trg_h-1)
				end
			end
		end
	}
	self.blt_type_count = #self.blt
	self.blt_sel_count = #self.blt
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

