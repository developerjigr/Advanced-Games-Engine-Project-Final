#include "Enemy.h"
#include "Player.h"


void CEnemy::Initialise(CGameObject& player, char* body, char* bodySkin, char* wep, char* wepSkin, CVector3f mPos, float w, float h, float d)
{

	mGameObjectID = 1;
	
	eCurrentState = IDLE;
	eCurrentAnimation = ANIM_IDLE;

	char* bSkin;
	char* wSkin;

	int random = rand() % 5;
	switch (random)
	{
	case 1:
		mSpeed = 3;
			bSkin = "Resources\\Meshes\\Yohko.bmp";
			wSkin = "Resources\\Meshes\\YohkoWeapon.bmp";
		break;
	case 2:
		mSpeed = 5;
		bSkin = "Resources\\Meshes\\Yohko.bmp";
		wSkin = "Resources\\Meshes\\YohkoWeapon.bmp";
		break;

	case 3:
		mSpeed = 10;
		bSkin = "Resources\\Meshes\\Yohko2.bmp";
		wSkin = "Resources\\Meshes\\YohkoWeapon.bmp";
		break;

	case 4:
		mSpeed = 12;
		bSkin = "Resources\\Meshes\\Yohko3.bmp";
		wSkin = "Resources\\Meshes\\YohkoWeapon.bmp";
		break;

	case 5:
		mSpeed = 17;
		bSkin = "Resources\\Meshes\\Yohko3.bmp";
		wSkin = "Resources\\Meshes\\YohkoWeapon.bmp";
		break;

	default:
		bSkin = bodySkin;
		wSkin = wepSkin;
		break;

	}

	mPhysics.Initialise(mPosition, CVector3f(0, 0, 0), 50.0f, 0.0f, w, h, d);


	mModel = CMD2Model();
	mModel.Load(body, bSkin, wep, wSkin);

	mPosition = CVector3f(mPos.x, 1.2f, mPos.z);
	mWidth = w; mHeight = h, mDepth = d;
	mPlayer = &player;

	mCollisionAudio.Initialise();
	mCollisionAudio.LoadEventSound(".\\Resources\\Audio\\Kick.wav");
	
	ammoID = rand() % 100000;

	mBloodTexture.Load(".\\Resources\\Textures\\bloodsplat.png", GL_REPEAT);
	mBloodTextureID = mBloodTexture.m_textureID;

	mRobotSpawn.Initialise();
	mRobotSpawn.LoadEventSound(".\\Resources\\Audio\\RobotSpawnSound.mp3");

	mRobotSpawn.PlayEventSound();

	mHealth = 100;

	//isActive = false;

}

void CEnemy::Update(float dt)
{

	randTimer = rand() % 100 * dt;

	if (shockwaveDeath.IsActive())
		shockwaveDeath.Update(dt);

	if (mMultiplierTimer.StartStopWatch(25))
	{
		mDeathCountMultiplier = 0;
	}

	if (!isActive)
		return;

	mElapsedTime += dt;

	if (CheckCollisions(ammoID) > 0)
	{
		UpdateState(HIT);
	}

	mDirection = mPlayer->GetPosition() - mPosition;
	mDirection.Normalise();
	mPhysics.Update(mPosition, mDirection, dt);

	switch (eCurrentState)
	{
	case IDLE:
		Idle();
		if (mStateSwitchTimer.StartStopWatch(randTimer))
		{
			UpdateState(WALK);
		}
		break;
	case WALK:
		Walk(dt);
		if (mStateSwitchTimer.StartStopWatch(randTimer))
		{
			UpdateState(IDLE);
		}
		break;
	case ATTACK:
		Attack();
		if (mStateSwitchTimer.StartStopWatch(randTimer))
		{
			UpdateState(WALK);
		}
		break;
	case HIT:
		Hit();
		if (mHealth <= 0)
			UpdateState(DEATH);
		if (mHitTimer.StartStopWatch(0.05f))
		{
			CanTakeDamage = true;
			UpdateState(WALK);
		}
		break;
	case DEATH:
		Death();
		break;
	default:
		Idle();
		break;
	}

	mCollisionAudio.Update();
	mRobotSpawn.Update();

}


void CEnemy::Render()
{

	glPushMatrix();
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	if (shockwaveDeath.IsActive())
		shockwaveDeath.Render();
	glPopMatrix();

	if (!isActive)
		return;

	mLookAngleY = atan2f(mDirection.x, mDirection.z) * (180 / PI_F);

	glPushMatrix();
		glEnable(GL_LIGHTING);
		glTranslatef(mPosition.x, mPosition.y, mPosition.z);
		glRotatef(mLookAngleY - 90, 0, 1, 0);
	//	glRotatef(mLookAngleX, 0, 1, 0);
		glScalef(0.05f, 0.05f, 0.05f); // Scale model to appropriate size in world
		mModel.Render(true);
		//glDisable(GL_LIGHTING);



	glPopMatrix();




	//mPhysics.DebugRayPoints();

}


void CEnemy::Death()
{
		if (mModel.IsLastFrameRendered())
		{
			mDeathCountMultiplier += 10;
			shockwaveDeath.Activate(0.15f, mPosition);
			DeactivateObject();
		}
}

void CEnemy::Idle()
{

	/*mPhysics.SetSpeed(0.0f);*/
	
}

void CEnemy::Hit()
{
	//CHANGE ANIMATION TO HIT AND STOP 
	
	if (CanTakeDamage) {
		if (mModel.IsLastFrameRendered()) {
			mSpeed += 2;
			ATTACK_DISTANCE += 4;
			//mPhysics.AddForce(50, -1 * mDirection);
			mHealth -= 10 + rand() % 40;
			mCollisionAudio.PlayEventSound();
			CanTakeDamage = false;
		}	
	}
	else {
		if (mHitTimer.StartStopWatch(0.15f))
		{
			CanTakeDamage = true;
			UpdateState(WALK);
		}
	}

	if (mHitTimer.StartStopWatch(0.15f))
	{
		CanTakeDamage = true;
		UpdateState(WALK);
	}

}

void CEnemy::Attack()
{
	if (CanAttack) {

		if (currentAmmo % (mAmmoCount) == 0)
			currentAmmo = 0;

		for (auto it = vBulletList->begin(); it != vBulletList->end(); ++it)
		{
			CBullet* c = *it;

			if (!c->isActive)
			{
				c->Initialise(mPosition, mDirection, mGameObjectID, 25, 50.0f, 0.125f, 0.125f, 0.25f);
				c->mGameObjectID = ammoID;
				c->SetActive(true);
				currentAmmo++;
				CanAttack = false;
				break;
			}
			else
			{
				continue;
			}
		}
		

	} else 
	{
		if (mAttackTimer.StartStopWatch(0.15f))
		{
			UpdateState(WALK);
		}
	}
}


void CEnemy::Walk(float dt)
{
	//Calc direction
	if ( abs(mPhysics.Distance(mPosition, mPlayer->GetPosition())) > 5 + rand() % ATTACK_DISTANCE ) {
		mPhysics.SetSpeed(mSpeed + 3, mDirection);
		mPosition = mPhysics.Update(mPosition, mDirection, dt);
	}
	else
	{
		CanAttack = true;
		UpdateState(ATTACK);
	}
}

void CEnemy::UpdateState(States newState)
{
	if (eCurrentState != newState) {
		switch (newState)
		{
		case IDLE:
			eCurrentState = newState;
			eCurrentAnimation = ANIM_IDLE;
			mModel.SetAnimation(eCurrentAnimation);
			break;
		case WALK:
			eCurrentState = newState;
			eCurrentAnimation = ANIM_WALK;
			mModel.SetAnimation(eCurrentAnimation);
			break;
		case HIT:
			eCurrentState = newState;
			eCurrentAnimation = ANIM_HIT1;
			mModel.SetAnimation(eCurrentAnimation);
			break;
		case ATTACK:
			eCurrentState = newState;
			eCurrentAnimation = ANIM_ATTACK;
			mModel.SetAnimation(eCurrentAnimation);
			break;
		case DEATH:
			eCurrentState = newState;
			eCurrentAnimation = ANIM_DEATH3;
			mModel.SetAnimation(eCurrentAnimation);
			break;

		default:
			break;

		}

	}



}