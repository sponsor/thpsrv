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
		tex_face_fine_msg = "�ǂ����ʂł���",
		tex_face_normal_x = 96,
		tex_face_normal_y = 160,
		tex_face_normal_msg = "���ʂ̌��ʂł���",
		tex_face_hurt_x = 192,
		tex_face_hurt_y = 160,
		tex_face_hurt_msg = "�������ʂł���",
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
-- �e�̏��
function BaseChara:GetBltInfo(num)
	return self.blt[num+1]:GetInfo()
end
function BaseBullet:GetInfo()
	return self.atk, self.delay, self.icon_x, self.icon_y, self.tex_x, self.tex_y, self.tex_w, self.tex_h, self.hit_range,self.bomb_range,self.ephemeris
end

-- �e�쐬
function BaseChara:ShotBullet(chr_obj_no,blt_type,px,py,vx,vy,vec_angle,power,frame,indicator_angle,indicator_power)
	return self.blt[blt_type+1]:Shot(chr_obj_no,self.id,blt_type,px,py,vx,vy,vec_angle,power,frame,indicator_angle,indicator_power)
end
-- power�ɓ���l�͂�0�`MAX_SHOT_POWER
-- 1=SOLID,2=GAS,4=LIQUID,ret true�Ŕ��ˏI��,false��Ԃ��Ƃ܂��Ă΂�frame���J�E���g�A�b�v���Ă���(�ő�250frame)
function BaseBullet:Shot(chr_obj_no,scr_id,blt_type,px,py,vx,vy,vec_angle,power,frame,indicator_angle,indicator_power)
	blt_no = C_CreateBullet(BLT_PROC_TYPE_SCR_CHARA,chr_obj_no,scr_id,blt_type,1,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,1,1)
	if blt_no ~= -1 then
		C_UpdateBulletAngle(blt_no,vec_angle)
	end
	return true
end
-- �X�y���쐬
function BaseChara:ShotSpell(chr_obj_no,px,py,vx,vy,vec_angle,power,frame,indicator_angle,indicator_power)
	return self.sc:Spell(chr_obj_no,self.id,px,py,vx,vy,vec_angle,power,frame,indicator_angle,indicator_power)
end
-- 1=SOLID,2=GAS,4=LIQUID,ret true�Ŕ��ˏI��,false��Ԃ��Ƃ܂��Ă΂�frame���J�E���g�A�b�v���Ă���(�ő�250frame)
function BaseBullet:Spell(chr_obj_no,scr_id,px,py,vx,vy,vec_angle,power,frame,indicator_angle,indicator_power)
	blt_no = C_CreateBullet(BLT_PROC_TYPE_SCR_SPELL,chr_obj_no,scr_id,0,1,px,py,vx,vy,self.add_vec_x,self.add_vec_y,self.hit_range,1,1)
	if blt_no ~= -1 then
		C_UpdateBulletAngle(blt_no,vec_angle)
	end
	return true
end

-- �n�ʂɓ��������F�e�^�C�v,�e������L����ObjNo,obj_no,�X�N���v�g�ԍ�,���������ʒux,y/�ړ��lx,y/�c��ړ�����0.0�`1.0/extdata
function BaseChara:OnHitStageBullet(blt_type,blt_chr_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
	return self.blt[blt_type+1]:OnHitStage(self.id,blt_type,blt_chr_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
end
function BaseBullet:OnHitStage(scr_id,blt_type,blt_chr_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
	C_RemoveBullet(blt_no,0) -- blt_no,rm_type(0:normal/1:out/2:bomb)
	C_BombObject(scr_id,blt_type,blt_chr_no,blt_no,hx,hy)
	return 1
end

-- �L�����ɓ��������F�e������L����ObjNo,chr_no,obj_no,�X�N���v�g�ԍ�,�e�^�C�v,���������ʒux,y/���������ӏ�x,y/�ړ��lx,y/�c��ړ�����0.0�`1.0/extdata
function BaseChara:OnHitCharaBullet(blt_type,blt_chr_no,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
	return self.blt[blt_type+1]:OnHitChara(self.id,blt_type,blt_chr_no,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
end
function BaseBullet:OnHitChara(scr_id,blt_type,blt_chr_no,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
	C_RemoveBullet(blt_no,2) -- obj_no,rm_type(0:normal/1:out/2:bomb)
	C_BombObject(scr_id,blt_type,blt_chr_no,blt_no,hx,hy)
	return 1
end

-- �e���e�ɓ��������F�e������L����ObjNo,hit_obj_no,obj_no,�X�N���v�g�ԍ�,�e�^�C�v,���������ʒux,y/���������ӏ�x,y/�ړ��lx,y/�c��ړ�����0.0�`1.0/extdata
function BaseChara:OnHitBulletBullet(blt_type,blt_chr_no,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
	return self.blt[blt_type+1]:OnHitBullet(self.id,blt_type,blt_chr_no,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
end
function BaseBullet:OnHitBullet(scr_id,blt_type,blt_chr_no,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
	return 0
end

-- �L�����������͈͓��������F�e������L����ObjNo,chr_no,obj_no,�X�N���v�g�ԍ�,�e�^�C�v,���������e�̈ʒux,y/�����ʒux,y/�͈͂̋߂��ɂ��З͒l/extdata
function BaseChara:OnHitCharaBulletBomb(blt_type,hit_chr_no,blt_chr_no,blt_no,hx,hy,power)
	self.blt[blt_type+1]:OnHitCharaBulletBomb(hit_chr_no,blt_chr_no,blt_no,hx,hy,power)
end
function BaseBullet:OnHitCharaBulletBomb(hit_chr_no,blt_chr_no,blt_no,hx,hy,power)
	C_DamageCharaHP(blt_chr_no,hit_chr_no,math.ceil(-self.atk*power))	-- �e������L����ObjNo,HP���炷�L����No,���炷��
--	C_UpdateCharaStatus(hit_chr_no,-self.atk*power,0,0)	-- chr_no,HP�����l,�ړ��l�����l,�f�B���C�����l
end

-- �e�̃t���[������
function BaseChara:OnFrameBullet(blt_type,blt_no,frame,px,py,vx,vy,ex1,ex2)
	self.blt[blt_type+1]:OnFrame(blt_type,blt_no,frame,px,py,vx,vy,ex1,ex2)
end
function BaseBullet:OnFrame(blt_type,blt_no,frame,px,py,vx,vy,ex1,ex2)
	return
end

-- �e�̋O������
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

-- SE�̃t�@�C����
function BaseChara:GetSEFilesCount()
	return #self.se
end

function BaseChara:GetSEFile(index)
	return self.se[index+1]
end

-- �e�̕`��
--	vec_angle ���ł����(�p�x)
--	state=DEF_STATE_MAIN_WAIT/DEF_STATE_MAIN_ACTIVE
function BaseChara:OnDrawBullet(blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
	self.blt[blt_type+1]:OnDraw(self.id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
end
function BaseBullet:OnDraw(scr_id,blt_type,blt_no,state,px,py,vx,vy,vec_angle,frame,ex1,ex2)
	C_UpdateBulletAngle(blt_no,vec_angle)
end
-- �e�̃^�C�v�ύX���̃C�x���g
function BaseChara:OnUpdateTypeCharaBullet(blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,type)
	self.blt[blt_type+1]:OnUpdateType(blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,type)
end
function BaseBullet:OnUpdateType(blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,type)
	return
end
-- �e�̏�ԕύX���̃C�x���g
function BaseChara:OnUpdateStateCharaBullet(blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,state)
	self.blt[blt_type+1]:OnUpdateState(blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,state)
end
function BaseBullet:OnUpdateState(blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,state)
	return
end
-- �������ύX�C�x���g
function BaseChara:OnChangeWindCharaBullet(blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,wind)
	self.blt[blt_type+1]:OnChangeWind(blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,wind)
end
function BaseBullet:OnChangeWind(blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,wind)
	return
end
-- �e�̃^�[���X�^�[�g���̃C�x���g
function BaseChara:OnTurnStartBullet(blt_type,blt_chr_no,blt_no,turn,ex1,ex2,act_obj_no)
	self.blt[blt_type+1]:OnTurnStart(blt_type,blt_chr_no,blt_no,turn,ex1,ex2,act_obj_no)
end
function BaseBullet:OnTurnStart(blt_type,blt_chr_no,blt_no,turn,ex1,ex2,act_obj_no)
	return
end
-- �e�̃^�[���G���h���̃C�x���g
function BaseChara:OnTurnEndBullet(blt_type,blt_chr_no,blt_no,turn,ex1,ex2,act_obj_no)
	self.blt[blt_type+1]:OnTurnEnd(blt_type,blt_chr_no,blt_no,turn,ex1,ex2,act_obj_no)
end
function BaseBullet:OnTurnEnd(blt_type,blt_chr_no,blt_no,turn,ex1,ex2,act_obj_no)
	return
end

-- �e�̔������̃C�x���g(���ɒe�������ꍇblt_no=-1,vx,vy,ex,ex=nil)
function BaseChara:OnBombCharaBullet(blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
	self.blt[blt_type+1]:OnBomb(blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
end
function BaseBullet:OnBomb(blt_type,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
	
end
-- spellcard ####################
-- �X�y���J�[�h�̏��
function BaseChara:GetSCBaseInfo()
	return self.sc:GetInfo()
end
-- ���O
function BaseChara:GetSCPartInfo()
	return self.sc:GetSCInfo()
end
function BaseBullet:GetSCInfo()
	return self.name,self.exp,self.exp_max
end

-- �X�y���̃t���[������
function BaseChara:OnFrameSpell(blt_no,frame,px,py,vx,vy,ex1,ex2)
	self.sc:OnFrame(DEF_BLT_TYPE_SPELL,blt_no,frame,px,py,vx,vy,ex1,ex2)
end

-- �e�̋O������
function BaseChara:OnGetEphemerisSpell(blt_no,frame,px,py,vx,vy,wind,ex1,ex2)
	return self.sc:OnGetEphemeris(blt_type,blt_no,frame,px,py,vx,vy,wind,ex1,ex2)
end

-- �X�y�����L�����ɓ�������
function BaseChara:OnHitCharaSpell(blt_chr_no,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
	return self.sc:OnHitChara(self.id,DEF_BLT_TYPE_SPELL,blt_chr_no,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
end

-- �X�y�����e�ɓ��������F�e������L����ObjNo,hit_obj_no,obj_no,�X�N���v�g�ԍ�,�e�^�C�v,���������ʒux,y/���������ӏ�x,y/�ړ��lx,y/�c��ړ�����0.0�`1.0/extdata
function BaseChara:OnHitBulletSpell(blt_chr_no,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
	return self.sc:OnHitBullet(self.id,DEF_BLT_TYPE_SPELL,blt_chr_no,hit_obj_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
end

-- �n�ʂɓ��������F�e������L����ObjNo,obj_no,�X�N���v�g�ԍ�,���������ʒux,y/�ړ��lx,y/�c��ړ�����0.0�`1.0/extdata
function BaseChara:OnHitStageSpell(blt_chr_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
	return self.sc:OnHitStage(self.id,DEF_BLT_TYPE_SPELL,blt_chr_no,blt_no,px,py,hx,hy,vx,vy,remain,frame,ex1,ex2)
end

-- �e�̃^�C�v�ύX���̃C�x���g
function BaseChara:OnUpdateTypeCharaSpell(blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,type)
	self.sc:OnUpdateType(DEF_BLT_TYPE_SPELL,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,type)
end

-- �e�̏�ԕύX���̃C�x���g
function BaseChara:OnUpdateStateCharaSpell(blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,state)
	self.sc:OnUpdateState(DEF_BLT_TYPE_SPELL,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,state)
end

-- �������ύX�C�x���g
function BaseChara:OnChangeWindCharaSpell(blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,wind)
	self.sc:OnChangeWind(DEF_BLT_TYPE_SPELL,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,wind)
end

-- �e�̃^�[���G���h���̃C�x���g
function BaseChara:OnTurnEndSpell(blt_chr_no,blt_no,turn,ex1,ex2,act_obj_no)
	self.sc:OnTurnEnd(DEF_BLT_TYPE_SPELL,blt_chr_no,blt_no,turn,ex1,ex2,act_obj_no)
end

-- �e�̃^�[���X�^�[�g���̃C�x���g
function BaseChara:OnTurnStartSpell(blt_chr_no,blt_no,turn,ex1,ex2,act_obj_no)
	self.sc:OnTurnStart(DEF_BLT_TYPE_SPELL,blt_chr_no,blt_no,turn,ex1,ex2,act_obj_no)
end

-- �e�̔������̃C�x���g(���ɒe�������ꍇblt_no=-1,vx,vy,ex,ex=nil)
function BaseChara:OnBombCharaSpell(blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
	self.sc:OnBomb(DEF_BLT_TYPE_SPELL,blt_no,blt_chr_no,px,py,vx,vy,ex1,ex2,bx,by,pxls)
end

-- �L�����������͈͓��������F�e������L����ObjNo,chr_no,obj_no,�X�N���v�g�ԍ�,�e�^�C�v,���������e�̈ʒux,y/�����ʒux,y/�͈͂̋߂��ɂ��З͒l/extdata
function BaseChara:OnHitCharaSpellBomb(hit_chr_no,blt_chr_no,blt_no,hx,hy,power)
	self.sc:OnHitCharaBulletBomb(hit_chr_no,blt_chr_no,blt_no,hx,hy,power)
end

--	vec_angle ���ł����(�p�x)
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

-- HP�ő�l
function BaseChara:GetHP()
	return self.max_hp
end

-- �e�̎��
function BaseChara:GetBltTypeCount()
	return self.blt_type_count,self.blt_sel_count
end
-- �A�C�R���̏��
function BaseChara:GetIcon()
	return self.tex_chr_x, self.tex_chr_y ,self.tex_chr_w, self.tex_chr_h
end

-- �e���ˉ��o return true�ŏI��
function BaseChara:OnTriggerFrame(type,px,py,angle,frame)
	if frame >= 25 then
		return true
	end
	return false
end

-- �^�[���I�����̃C�x���g
function BaseChara:OnTurnEndChara(chr_no,turn,ex1,ex2)

end

-- �^�[���J�n���̃C�x���g
function BaseChara:OnTurnStartChara(chr_no,turn,ex1,ex2)

end

-- �L������HP���ύX�����F�L������ObjNo,max hp, current hp,�ύX�����HP/extdata
function BaseChara:OnChangeCharaHP(chr_no,max_hp,current_hp,new_hp,ex1,ex2)
	return new_hp		-- �ύX�����HP�Ƃ͈Ⴄ�l�ɂ������ꍇ�Areturn����l��ς���
end

-- ���������񂾎��̃C�x���g(DEF_CHARA_DEAD_KILL/DEF_CHARA_DEAD_DROP/DEF_CHARA_DEAD_CLOSE)
-- prv_stt ���O�̃L�������
function BaseChara:OnDeadChara(chr_no,type,prv_stt)
--	if prv_stt ~= DEF_STATE_DEAD then
		if type ~= DEF_CHARA_DEAD_DROP then
			chr = C_GetCharacterFromObjNo(chr_no)
			if chr~=nil then
				if chr.angle <= 90 or chr.angle >= 270 then			-- ���������Ă���ꍇ
					C_SetCharaTexture(chr_no,64,64,96,96)
				else
					C_SetCharaTexture(chr_no,32,64,64,96)
				end
			end
		end
--	end
end
-- �L���������[�h���ꂽ���̃C�x���g
function BaseChara:OnLoadChara(chr_no,px,py)

end

-- �L�����`��C�x���g
function BaseChara:OnDrawChara(chr_no,state,angle,vx,vy,frame)
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
	elseif state==DEF_STATE_TRIGGER_BULLET or state==DEF_STATE_TRIGGER_SPELL then
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
	end
end

