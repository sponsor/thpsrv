module("data.scr.chr.002_mokou",package.seeall)

-- chara
Chara = {}

function Chara.new()
	local CharaID = 2
	local bc = require("data.scr.BaseChara")
	local blt1  = {
		id = CharaID,
		hit_range = 8,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 32,
		atk = 270,
		delay = 130,
		icon_x = 96,
		icon_y = 64,
		tex_x = 96,
		tex_y = 64,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/002_mokou/mokou_b10.wav",
				"data/scr/chr/002_mokou/mokou_b11.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			e=math.random(8,16)
			for i=0,e do
				effect_no = C_AddEffect(self.id,0,96,64,160,bx,by,40)
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
			C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,1,1)
			C_PlaySoundSE(self.se[1],0,0)
			return true
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			if math.random(0,1) == 1 then
				ang = math.random(45,135)
				evx = -math.cos(math.rad(ang))
				evy = -math.sin(math.rad(ang))
				effect_no = C_AddEffect(self.id,96,64,128,96,px+math.random(12)-6,py-4,10)
				if effect_no ~= -1 then
					C_SetEffectScale(effect_no,0.75,0.75)
					C_SetEffectScalling(effect_no,-0.05,-0.05)
					C_SetEffectFade(effect_no,-math.random(25,30))
					vs = (math.random()*2+2)
					C_SetEffectVector(effect_no, vs*evx,vs*evy,-evx*0.05,-evy*0.05+0.02)
				end
			end
		end,
	}
	setmetatable(blt1,{ __index = bc.BaseBullet.new()})

	-- blt2����莞�Ԍo���y�􂵂�blt0�̒e4�ɂȂ�
	local blt0 = {
		id = CharaID,
		hit_range = 5,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 19,
		atk = 56,
		delay = 180,
		icon_x = 160,
		icon_y = 64,
		tex_x = 160,
		tex_y = 64,
		tex_w = 32,
		tex_h = 32,
		se = {	"data/scr/chr/002_mokou/mokou_b00.wav",
				"data/scr/chr/002_mokou/mokou_b01.wav"},
		Shot = function(self,chr_obj_no,chr_id,blt_type,px,py,vx,vy,vec_angle,power,frame)
			e = math.random(4,6)
			for i=0,e do
				rx = math.random(80,240)-120
				ry = math.random(80,240)-120
				blt_no = C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,vx+rx,vy+ry,self.add_vec_x,self.add_vec_y,self.hit_range,1,1)
			end
			for i=0,18 do
				effect_no = C_AddEffect(chr_id,64,96,128,160,px,py,20)
				if effect_no ~= -1 then
					ang = math.random(0,359)
					evx = math.cos(math.rad(ang))
					evy = math.sin(math.rad(ang))
					rnd = math.random(40,75)*0.005
					C_SetEffectScale(effect_no,rnd,rnd)
					C_SetEffectRotation(effect_no, 10)
					C_SetEffectFade(effect_no,-math.random(15,25))
					vs = (math.random()*8+4)
					C_SetEffectVector(effect_no, vs*evx,vs*evy,-evx*rnd,-evy*rnd+0.02)
				end
			end
			C_PlaySoundSE(self.se[1],0,0)
			return true
		end,
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			effect_no = C_AddEffect(self.id,0,480,32,512,bx,by,25)
			if effect_no ~= -1 then
				C_SetEffectAnimation(effect_no,2,12,false)
			end
		end
	}
	setmetatable(blt0,{ __index = bc.BaseBullet.new()})
	
	local blt2 = {
		id = CharaID,
		hit_range = 6,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 43,
		atk = 250,
		delay = 190,
		icon_x = 128,
		icon_y = 64,
		tex_x = 64,
		tex_y = 96,
		tex_w = 64,
		tex_h = 64,
		se = {	"data/scr/chr/002_mokou/mokou_b20.wav",
				"data/scr/chr/002_mokou/mokou_b21.wav"},
		OnBomb = function(self,blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
			C_PlaySoundSE(self.se[2],0,0)
			effect_no = C_AddEffect(self.id,0,480,32,512,bx,by,36)
			if effect_no ~= -1 then
				C_SetEffectScale(effect_no,2.75,2.75)
				C_SetEffectAnimation(effect_no,3,12,false)
			end
		end,
		Shot = function(self,chr_obj_no,chr_id,blt_type,px,py,vx,vy,vec_angle,power,frame)
			HFPS = FPS/2
			C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,chr_id,blt_type,1,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,((power/DEF_MAX_SHOT_POWER)*HFPS*0.5)+(HFPS*0.8),1)
			C_PlaySoundSE(self.se[1],0,0)
			return true
		end,
		OnFrame = function(self,blt_type,blt_no,frame,px,py,vx,vy,ex1,ex2)
			if frame >= ex1 then
				b = C_GetBulletInfo(blt_no)
				if C_RemoveBullet(blt_no,0) == true then
					blt0:Shot(b.chr_obj_no,self.id,2,b.ax,b.ay,b.vx,b.vy,b.angle,1,0)
				end
			end
		end,
		OnDraw = function(self,scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
			if vx > 0 then
				C_UpdateBulletAngle(blt_no,frame*20)
			else
				C_UpdateBulletAngle(blt_no,frame*20)
			end
			if math.random(0,2) == 1 then
				effect_no = C_AddEffect(scr_id,self.tex_x,self.tex_y,self.tex_x+self.tex_w,self.tex_y+self.tex_h,px,py,25)
				if effect_no ~= -1 then
					ang = math.random(0,359)
					evx = math.cos(math.rad(ang))
					evy = math.sin(math.rad(ang))
					rnd = math.random(40,75)*0.005
					C_SetEffectScale(effect_no,rnd,rnd)
					C_SetEffectRotation(effect_no, 10)
					C_SetEffectFade(effect_no,-math.random(10,20))
					vs = (math.random()*8+4)
					C_SetEffectVector(effect_no, vs*evx,vs*evy,-evx*rnd,-evy*rnd)
				end
			end
		end,
	}
	setmetatable(blt2,{ __index = bc.BaseBullet.new()})

	local spellcard = {
		id = CharaID,
		name = "�u���U���N�V�����v",
		exp = 600,
		exp_max = 1000,
		hit_range = 24,
		add_vec_x = 0,
		add_vec_y = 20,
		bomb_range= 0,
		atk = 100,
		delay = 150,
		icon_x = 192,
		icon_y = 64,
		tex_x = 192,
		tex_y = 64,
		tex_w = 32,
		tex_h = 32,
		se = {},
		Spell = function(self,chr_obj_no,chr_id,px,py,vx,vy,vec_angle,power,frame)
			h = (DEF_MAX_SHOT_POWER/2)
			ex1 = math.max(0,5-(math.abs(power-h)/20))	-- �^�񒆂���̋����Ōv�Z
			if ex1 > 0 then
				blt_no=C_CreateBullet(BLT_PROC_TYPE_SCR_SPELL,chr_obj_no,chr_id,DEF_BLT_TYPE_SPELL,OBJ_TYPE_TACTIC,px,py,0,0,0,0,0,ex1,0)
				if blt_no ~= -1 then
					C_UpdateBulletState(blt_no,DEF_STATE_WAIT)
				end
				-- ���L�����̃��O�ɉ񕜗ʂ̃��b�Z�[�W���o��
				if C_GetMyCharaNo() == chr_obj_no then
					msg = string.format("���^�[���I������ %d �� 50�񕜂��܂�",ex1)
					C_AddMsgLog(msg)
				end
			else
				if C_GetMyCharaNo() == chr_obj_no then
					msg = string.format("�X�y���J�[�h�����Ɏ��s���܂���",ex1)
					C_AddMsgLog(msg)
				end
		
			end
			return true
		end,
		OnTurnEnd = function(self,blt_type,blt_chr_no,blt_no,turn,ex1,ex2,act_obj_no)
			if blt_chr_no == act_obj_no then
				if ex1 <= 0 then
					C_RemoveBullet(blt_no,0)
				elseif blt_chr_no == act_obj_no then
					C_SetBulletExtData1(blt_no, ex1-1)
					C_UpdateCharaStatus(blt_chr_no,50,0,0,0)	-- �L����ObjNo,hp,mv,delay,exp
				end
			end
		end,
	}
	setmetatable(spellcard,{ __index = bc.BaseBullet.new()})
	
	self = {
		id = CharaID,
		name = "�������g",
		tex_chr = "data/scr/chr/002_mokou/mokou.png",
		angle_range_min = 10,
		angle_range_max = 70,
		move = 90,
		delay = 530,
		max_hp = 900,
		draw_w = 45,
		draw_h = 45,
		tex_chr_num = 3,
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
		tex_face_fine_msg = "�����Ă���Ηǂ������������ˁI",
		tex_face_normal_msg = "�����Ă邾���ł��[����",
		tex_face_hurt_msg = "����Ȃ̉i���̐��̒��ł͍��ׂȎ���",
		blt = {blt1, blt2, blt0},
		sc = spellcard,
		se = {	"data/se/spell00.wav"},
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
					effect_no = C_AddBGEffect(self.id,308,0,512,480,bgx,320,40)
					if effect_no ~= -1 then
						C_SetBGEffectFadeInOut(effect_no,10)
						C_SetBGEffectScale(effect_no,2,2)
						C_SetBGEffectVector(effect_no, 0,4,0,0)
					end
				elseif frame >= 45 then
					return true
				elseif frame > 4 and frame < 40 then
					e=math.random(1,3)
					if e == 2 then
						fx = math.random(0,64)-32
						fy = math.random(0,64)-32
						tex = math.random(1,2)
						if tex == 1 then
							effect_no = C_AddEffect(self.id,128,96,192,160,px+fx,py+fy,math.random()*(50-frame))
						else
							effect_no = C_AddEffect(self.id,192,96,256,160,px+fx,py+fy,math.random()*(50-frame))
						end
						if effect_no ~= -1 then
							scale = math.random()*0.5+0.25
							C_SetEffectAlpha(effect_no,math.random(96,224))
							C_SetEffectFadeInOut(effect_no,math.random(5,10))
							C_SetEffectScale(effect_no,scale,scale)
							C_SetEffectVector(effect_no, 0,-math.random()*1.5-0.5,0,0)
						end
						bgx = 600
						stgw = C_GetStageWidth()
						if (stgw/2) <= px then	-- �X�e�[�W�̉E���̏ꍇ�͍����ɕ\��
							bgx = 200
						end
						fx = math.random(0,256)+bgx-128
						fy = math.random(60,440)
						if tex == 1 then
							effect_no = C_AddBGEffect(self.id,128,96,192,160,fx,fy,math.random()*(50-frame))
						else
							effect_no = C_AddBGEffect(self.id,192,96,256,160,fx,fy,math.random()*(50-frame))
						end
						if effect_no ~= -1 then
							scale = math.random()*2+0.75
							C_SetBGEffectAlpha(effect_no,math.random(96,224))
							C_SetBGEffectFadeInOut(effect_no,math.random(5,10))
							C_SetBGEffectScale(effect_no,scale,scale)
							C_SetBGEffectVector(effect_no, 0,-math.random()*2.5-0.5,0,0)
						end
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
				-- 1�t���[���ڂ���
				tex_idx = 0
				tex_left = self.tex_trg_x					-- �����ʒu��ێ�
				if angle <= 90 or angle >= 270 then			-- ���������Ă���ꍇ
					tex_idx = self.tex_trg_num				-- �e�N�X�`�������C���f�b�N�X�����炷
				end
				tex_left = self.tex_trg_x + self.tex_trg_w * tex_idx	-- �e�N�X�`�����[���v�Z
				C_SetCharaTexture(chr_no,tex_left+1,self.tex_trg_y+1,tex_left+self.tex_trg_w-1,self.tex_trg_y+self.tex_trg_h-1)
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
	return setmetatable(self,{ __index = bc.BaseChara.new()})
end


