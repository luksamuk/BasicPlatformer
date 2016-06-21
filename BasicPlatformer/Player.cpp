#include "Player.hpp"
#include "Solid.hpp"
#include "Grid.hpp"
#include <sstream>

Player::Player()
{
	SetName("Player");
	m_spawner = nullptr;
}


Player::~Player()
{
}

bool Player::OnGround() {
	return ground;
}

vec2 Player::Speed() {
	return m_velocity;
}

vec2 Player::GroundSpeed() {
	return m_groundvelocity;
}

void Player::Initialize()
{
	SetPosition(vec2(180.0f, 330.0f));
	ground = true;
	pressingJump = false;
	jumpAngle = 0.0f;
	values = &default_values;
	m_currentState = PLAYER_DEFAULT;
	soundEmitter = new OficinaFramework::AudioSystem::AudioSource;
	m_currentAction = PLAYER_NOACTION;
	m_direction = 1.0f;
	m_super = false;
	m_underwater = false;
	m_speedshoes = false;
	m_haswater = false;
	m_viewsensors = false;
	m_grid = nullptr;
}

void Player::Update()
{
	/* SENSOR REPOSITIONING */
	{
		m_leftSensor = m_rightSensor
			= m_bottomSensorL = m_bottomSensorR
			= m_topSensorL = m_topSensorR
			= m_position;

		// Side sensors should be a little lower
		m_leftSensor.y = m_rightSensor.y += m_sidesensors_ydist;

		// Bottom sensors should be a little to the sides
		m_bottomSensorL.x -= m_vertsensor_dist;
		m_bottomSensorR.x += m_vertsensor_dist;
		// Same with top
		m_topSensorL.x -= m_vertsensor_dist;
		m_topSensorR.x += m_vertsensor_dist;

		// Height control
		if (m_currentAction == PLAYER_JUMPING
			|| m_currentAction == PLAYER_ROLLING
			|| m_currentAction == PLAYER_CROUCHED
			|| m_currentAction == PLAYER_SPINDASH)
			m_hitboxHeight = m_hitboxRadius - 12.0f;
		else m_hitboxHeight = m_hitboxRadius;

		m_vlinecast_top = m_hitboxHeight + 16.0f;
	}


	/* VELOCITY TRANSFORMATION */
	// Translate real velocity to engine
	m_velocity.x = m_groundvelocity.x * -cosf(M_PI + m_angle);
	m_velocity.x += m_groundvelocity.y * sinf(M_PI + m_angle);
	m_velocity.y = m_groundvelocity.y * -cosf(M_PI + m_angle);
	m_velocity.y += m_groundvelocity.x * -sinf(M_PI + m_angle);

	// Transform position
	m_position.x += m_velocity.x;
	m_position.y += m_velocity.y;


	/* INPUT */
	vec2 leftStick = OficinaFramework::InputSystem::GetLeftStick();

	/* VERTICAL MOVEMENT */
	// Gravity
	if (!ground) m_groundvelocity.y += values->m_gravity;

	// Air drag
	if (m_groundvelocity.y < 0.0f && m_groundvelocity.y > values->m_airdrag_minyv
		&& abs(m_groundvelocity.x) >= values->m_airdrag_minxv)
		m_groundvelocity.x *= values->m_airdragfactor;




	/* HORIZONTAL MOVEMENT */
	// By NO MEANS could Sonic accelerate while rolling, crouching, looking up or spindashing.
	if (m_currentAction != PLAYER_ROLLING
		&& m_currentAction != PLAYER_CROUCHED
		&& m_currentAction != PLAYER_LOOKUP
		&& m_currentAction != PLAYER_SPINDASH)
	{
		if ((leftStick.x > 0.0f && !rightwall)
			|| (leftStick.x < 0.0f && !leftwall))
		{
			if (abs(m_groundvelocity.x) < values->m_topXSpeed)
				m_groundvelocity.x += leftStick.x * (ground ? values->m_accel : values->m_airaccel);
		}
	}
	
	// Horizontal friction when going up slopes
	m_groundvelocity.x += (values->m_slopefactor * sinf(m_angle));

	// Horizontal friction while rolling
	if (m_currentAction == PLAYER_ROLLING)
	{
		if (ground)
		{
			// Keep rolling if absolute speed is greater than unroll value
			if (m_groundvelocity.x > values->m_unrollMinXSpeed)
				m_groundvelocity.x -= values->m_rollingfriction;
			else if (m_groundvelocity.x < -values->m_unrollMinXSpeed)
				m_groundvelocity.x += values->m_rollingfriction;
			// If absolute speed is lower or equal unroll value, then unroll
			else
			{
				//m_groundvelocity.x = 0.0f;
				m_currentAction = PLAYER_NOACTION;
			}
		}
	}
	// Horizontal friction
	else
	{
		if (ground && leftStick.x == 0.0f)
		{
			if (abs(m_groundvelocity.x) >= values->m_friction) {
				if (m_groundvelocity.x > 0.0f) m_groundvelocity.x -= values->m_friction;
				else m_groundvelocity.x += values->m_friction;
			}
			else m_groundvelocity.x = 0.0f;
		}
	}


	// Horizontal deceleration
	if (ground)
	{
		// If Sonic is rolling, you CAN decelerate.
		// Uncurling is defined on friction section.
		if (m_currentAction == PLAYER_ROLLING) {
			if (leftStick.x < 0.0f
				&& m_groundvelocity.x >= values->m_unrollMinXSpeed)
				m_groundvelocity.x -= values->m_rollingdecel;
			else if (leftStick.x > 0.0f
				&& m_groundvelocity.x <= -values->m_unrollMinXSpeed)
				m_groundvelocity.x += values->m_rollingdecel;
		}
		// In case Sonic isn't rolling, it is perfectly possible to perform
		// a break.
		else
		{
			if (leftStick.x < 0.0f && m_groundvelocity.x > 0.0f)
			{
				if (m_currentAction != PLAYER_SKIDDING && abs(m_groundvelocity.x) > 1.8f) {
					m_currentAction = PLAYER_SKIDDING;
					soundEmitter->Stop();
					soundEmitter->Play(sfx_00_skidding);
				}

				if (m_groundvelocity.x >= values->m_decel)
					m_groundvelocity.x -= values->m_decel;
				else m_groundvelocity.x = 0.0f;
			}
			else if (leftStick.x > 0.0f && m_groundvelocity.x < 0.0f)
			{
				if (m_currentAction != PLAYER_SKIDDING && abs(m_groundvelocity.x) > 1.8f) {
					m_currentAction = PLAYER_SKIDDING;
					soundEmitter->Stop();
					soundEmitter->Play(sfx_00_skidding);
				}

				if (m_groundvelocity.x <= -values->m_decel)
					m_groundvelocity.x += values->m_decel;
				else m_groundvelocity.x = 0.0f;
			}

			if (m_currentAction == PLAYER_SKIDDING)
			{
				if ((leftStick.x >= 0.0f && m_groundvelocity.x >= 0.0f)
					|| (leftStick.x <= 0.0f && m_groundvelocity.x <= 0.0f)) {
					m_currentAction = PLAYER_NOACTION;
				}
			}
		}
	}



	/* DIRECTION CHANGING */
	// Only works if not looking up, crouching nor spindashing
	if (m_currentAction != PLAYER_CROUCHED
	&& m_currentAction != PLAYER_LOOKUP
	&& m_currentAction != PLAYER_SPINDASH)
	{
		if (ground)
		{
			if (m_groundvelocity.x > 0.0f && leftStick.x > 0.0f)
				m_direction = 1.0f;
			else if (m_groundvelocity.x < 0.0f && leftStick.x < 0.0f)
				m_direction = -1.0f;
		}
		else if (leftStick.x != 0.0f) m_direction = leftStick.x / abs(leftStick.x);
	}





	/* SPEED LIMITS */
	// When rolling (on ground, since rolling on air is jump), we only cap to top X speed
	if(ground && m_currentAction == PLAYER_ROLLING)
		clamp(m_groundvelocity.x, -values->m_rollingTopXSpeed, values->m_rollingTopXSpeed);
	// On other actions, original values are conserved
	else clamp(m_groundvelocity.x, -values->m_maxXSpeed, values->m_maxXSpeed);





	/* COLLISION DETECTION */
	bool foundGround  = false,
		 foundCeiling = false;

	Solid* bottomSolidL = nullptr,
		*bottomSolidR = nullptr;
		
	//	 * topSolidL = nullptr,
	//	 * topSolidR = nullptr;

	/*float bottomSensorLLastHeight = 16.0f,
		  bottomSensorRLastHeight = 16.0f,
		
		  topSensorLLastHeight = 16.0f,
		  topSensorRLastHeight = 16.0f;*/

	vec2 aabbPos(m_position.x - 32.0f, m_position.y - m_vlinecast);
	vec2 aabbSize(64.0f, m_vlinecast * 2.0f);

	float heightBuffer;
	leftwall = rightwall = false;
	//int objs_iterated = 0, solids_iterated = 0, valid_solids = 0;
	
	//if (m_grid)
	{
		/*std::vector<OficinaFramework::EntitySystem::Entity*> myEntities;

		vec2 cellSize = m_grid->getCellSize();

		std::vector<OficinaFramework::EntitySystem::Entity*>* cells[] = {
			m_grid->GetNearest(m_position),
			m_grid->GetNearest(vec2(m_position.x - cellSize.x, m_position.y)),
			m_grid->GetNearest(vec2(m_position.x + cellSize.x, m_position.y)),
			m_grid->GetNearest(vec2(m_position.x, m_position.y + cellSize.y)),
			m_grid->GetNearest(vec2(m_position.x - cellSize.x, m_position.y + cellSize.y)),
			m_grid->GetNearest(vec2(m_position.x + cellSize.x, m_position.y + cellSize.y)),
			m_grid->GetNearest(vec2(m_position.x, m_position.y - cellSize.y)),
			m_grid->GetNearest(vec2(m_position.x - cellSize.x, m_position.y - cellSize.y)),
			m_grid->GetNearest(vec2(m_position.x + cellSize.x, m_position.y - cellSize.y))
		};

		for (int i = 0; i < 9; i++)
			if (cells[i] != nullptr)
				myEntities.insert(myEntities.end(), cells[i]->begin(), cells[i]->end());*/

		for (auto obj : *GetParent())
		{
			//objs_iterated++;
			if (obj->GetName() != "Solid") continue;
			//solids_iterated++;
			Solid* mySolid = (Solid*)obj;
			if (!mySolid->Intersects(aabbPos, aabbSize)) continue;
			//valid_solids++;

			// Side collision
			// Left
			if (mySolid->GetProperty(1) == false
				&& mySolid->Linecast(m_leftSensor, vec2::Left(), m_hlinecast))
			{
				if (m_direction < 0.0f) leftwall = true;
				if (m_groundvelocity.x < 0.0f
					&& m_position.x - m_hlinecast < mySolid->Right())
				{
					m_groundvelocity.x = 0.0f;
					m_position.x = mySolid->Right() + m_hlinecast;
					continue; // Prevent bottom/ceiling from colliding
				}
			}
			// Right
			if (mySolid->GetProperty(1) == false
				&& mySolid->Linecast(m_rightSensor, vec2::Right(), m_hlinecast))
			{
				if (m_direction > 0.0f) rightwall = true;
				if (m_groundvelocity.x > 0.0f
					&& m_position.x + m_hlinecast > mySolid->Left())
				{
					m_groundvelocity.x = 0.0f;
					m_position.x = mySolid->Left() - m_hlinecast;
					continue; // Prevent bottom/ceiling from colliding
				}
			}

			// Bottom collision
			// Bottom Left
			if (m_groundvelocity.y >= 0.0f &&
				(heightBuffer = mySolid->Linecast(m_bottomSensorL, vec2::Down(), m_vlinecast)))
			{
				if ((m_position.y + m_hitboxRadius > mySolid->Top(m_bottomSensorL.x))
					|| ground == true)
				{
					foundGround = true;
					if (!bottomSolidL || mySolid->GetPosition().y <= bottomSolidL->GetPosition().y)
						bottomSolidL = mySolid;
				}
			}
			// Bottom Right
			if (m_groundvelocity.y >= 0.0f &&
				(heightBuffer = mySolid->Linecast(m_bottomSensorR, vec2::Down(), m_vlinecast)))
			{
				if ((m_position.y + m_hitboxRadius > mySolid->Top(m_bottomSensorR.x))
					|| ground == true)
				{
					foundGround = true;
					if (!bottomSolidR || mySolid->GetPosition().y <= bottomSolidR->GetPosition().y)
						bottomSolidR = mySolid;
				}
			}

			// Top collision
			// Top Left
			if (mySolid->GetProperty(1) == false
				&& (heightBuffer = mySolid->Linecast(m_topSensorL, vec2::Up(), m_vlinecast_top)))
			{
				if (m_position.y - m_hitboxHeight < mySolid->Bottom())
				{
					foundCeiling = true;
					//if (!topSolidL || mySolid->GetPosition().y >= topSolidL->GetPosition().y)
					//	topSolidL = mySolid;
				}
			}
			// Top Right
			if (mySolid->GetProperty(1) == false
				&& (heightBuffer = mySolid->Linecast(m_topSensorR, vec2::Up(), m_vlinecast_top)))
			{
				if (m_position.y - m_hitboxHeight < mySolid->Bottom())
				{
					foundCeiling = true;
					//if (!topSolidR || mySolid->GetPosition().y >= topSolidR->GetPosition().y)
					//	topSolidR = mySolid;
				}
			}
		}
	}

	//printf("Iterated through %d objects, where %d are solids (%d were valid).\n", objs_iterated, solids_iterated, valid_solids);

	// If bottom collision not found
	if (!foundGround) {
		ground = false;
		// conservative values
		{
			vec2 oldGroundVelocity = m_groundvelocity;
			m_groundvelocity.x  = oldGroundVelocity.x * -cosf(M_PI + m_angle);
			m_groundvelocity.x += oldGroundVelocity.y *  sinf(M_PI + m_angle);
			m_groundvelocity.y  = oldGroundVelocity.y * -cosf(M_PI + m_angle);
			m_groundvelocity.y += oldGroundVelocity.x * -sinf(M_PI + m_angle);
		}
		m_angle = 0.0f;
	}
	else {
		// Disable jump
		if (m_currentAction == PLAYER_JUMPING)
			m_currentAction = PLAYER_NOACTION;

		float biggestHeight, biggestsAngle;

		// Determine biggest height and its angle
		if (bottomSolidL && bottomSolidR) {
			float lTop = bottomSolidL->Top(m_bottomSensorL.x),
				  rTop = bottomSolidR->Top(m_bottomSensorR.x);
			if (lTop <= rTop) {
				biggestHeight = lTop;
				biggestsAngle = bottomSolidL->angularCoefficient(m_bottomSensorL.x);
			}
			else {
				biggestHeight = rTop;
				biggestsAngle = bottomSolidR->angularCoefficient(m_bottomSensorR.x);
			}
		}
		else {
			if (bottomSolidL) {
				biggestHeight = bottomSolidL->Top(m_bottomSensorL.x);
				biggestsAngle = bottomSolidL->angularCoefficient(m_bottomSensorL.x);
			}
			if (bottomSolidR) {
				biggestHeight = bottomSolidR->Top(m_bottomSensorR.x);
				biggestsAngle = bottomSolidR->angularCoefficient(m_bottomSensorR.x);
			}
		}

		// Finally set position
		m_position.y = biggestHeight - m_hitboxRadius;
		m_angle = atan(biggestsAngle);
		m_groundvelocity.y = 0.0f;
		ground = true;
	}

	// If top collision found
	if (!foundCeiling) ceiling = false;
	else
	{
		if (m_groundvelocity.y < 0.0f)
		{
			/*if (topSolidL && topSolidR)
				m_position.y = fmax(topSolidL->Bottom(), topSolidR->Bottom()) + m_vlinecast;
			else if (topSolidL)
				m_position.y = topSolidL->Bottom() + m_vlinecast;
			else m_position.y = topSolidR->Bottom() + m_vlinecast;*/

			m_groundvelocity.y = 0.0f;
			ceiling = true;
		}
	}

	// Left boundary as left collision
	if (m_position.x <= m_hlinecast && m_groundvelocity.x < 0.0f) {
		m_groundvelocity.x = 0.0f;
		m_position.x = m_hlinecast;
	}






	/* ACTIONS */

	// Jump
	pressingJump = OficinaFramework::InputSystem::PressingButton(OficinaFramework::InputSystem::GamePadButton::A);
	// Trigger jump
	if(ground
		&& m_currentAction != PLAYER_JUMPING
		&& m_currentAction != PLAYER_CROUCHED
		&& m_currentAction != PLAYER_SPINDASH
		&& (!ceiling)
		&& OficinaFramework::InputSystem::PressedButton(OficinaFramework::InputSystem::GamePadButton::A))
	{
		soundEmitter->Stop();
		soundEmitter->Play(sfx_02_jump);
		m_currentAction = PLAYER_JUMPING;
		ground = false;
		jumpAngle = m_angle;
		m_xSpeedBeforeJump = m_groundvelocity.x;
		m_groundvelocity.x += values->m_jump_strength * -sinf(jumpAngle);
		m_groundvelocity.y += values->m_jump_strength *  cosf(jumpAngle);
	}
	// Minimal jump
	if (!ground && !pressingJump && m_groundvelocity.y < values->m_min_jump_str)
		m_groundvelocity.y = values->m_min_jump_str * cosf(jumpAngle);

	// Rolling
	if(ground
		&& m_currentAction != PLAYER_ROLLING
		&& abs(m_groundvelocity.x) >= values->m_rollingMinXSpeed
		&& leftStick.y > 0.0f) {
		soundEmitter->Stop();
		soundEmitter->Play(sfx_01_rolling);
		m_currentAction = PLAYER_ROLLING;
	}
	// If on air and rolling, then you're technically jumping.
	// This will also allow you to regain control.
	if (!ground && m_currentAction == PLAYER_ROLLING)
		m_currentAction = PLAYER_JUMPING;

	// Crouch down & Look up
	if (ground
		&& m_groundvelocity == 0.0f
		&& (m_currentAction == PLAYER_NOACTION
			|| m_currentAction == PLAYER_CROUCHED
			|| m_currentAction == PLAYER_LOOKUP))
	{
		// Crouch down
		if(leftStick.y > 0.0f) m_currentAction = PLAYER_CROUCHED;
		// Look up
		else if (leftStick.y < 0.0f) m_currentAction = PLAYER_LOOKUP;
		// Restore
		else m_currentAction = PLAYER_NOACTION;
	}

	// Just in case.
	if ((m_currentAction == PLAYER_CROUCHED
		|| m_currentAction == PLAYER_LOOKUP))
	{
		if(leftStick.y == 0.0f || m_groundvelocity.x != 0.0f)
			m_currentAction = PLAYER_NOACTION;
	}

	// Spindash
	if (ground
		&& (m_currentAction == PLAYER_CROUCHED
			|| m_currentAction == PLAYER_SPINDASH))
	{
		// Activate
		if (m_currentAction == PLAYER_CROUCHED
			&& OficinaFramework::InputSystem::PressedButton(OficinaFramework::InputSystem::GamePadButton::A))
		{
			m_currentAction = PLAYER_SPINDASH;
			soundEmitter->Stop();
			soundEmitter->Play(sfx_03_spindash);
		}
		else if (m_currentAction == PLAYER_SPINDASH)
		{
			// If pressed A, then rev
			if (OficinaFramework::InputSystem::PressedButton(OficinaFramework::InputSystem::GamePadButton::A))
			{
				m_spindashRevolutions += 2.0f;
				soundEmitter->Stop();
				soundEmitter->Play(sfx_03_spindash);
			}

			// Update revolutions
			m_spindashRevolutions -= truncf(m_spindashRevolutions / 0.125f) / 256.0f;
			clamp(m_spindashRevolutions, 0.0f, 8.0f);

			// If released, then spin with appropriate speed
			if (leftStick.y <= 0.0f)
			{
				m_groundvelocity.x = m_direction * (8.0f + (floor(m_spindashRevolutions) / 2.0f));
				m_currentAction = PLAYER_ROLLING;
				m_spindashRevolutions = 0.0f;
				soundEmitter->Stop();
				soundEmitter->Play(sfx_04_release);
			}
		}
	}


	/* VALUE STATE CONTROL */
	const PlayerValues* desiredValues = nullptr;
	switch (m_currentState) {
		default: m_currentState = PLAYER_DEFAULT;
		case PLAYER_DEFAULT:          desiredValues = &default_values;          break;
		case PLAYER_SPEEDSHOES:       desiredValues = &speedshoes_values;       break;
		case PLAYER_UNDERWATER:       desiredValues = &underwater_values;       break;
		case PLAYER_SUPER:            desiredValues = &super_values;            break;
		case PLAYER_SUPER_UNDERWATER: desiredValues = &super_underwater_values; break;
	}
	if (values != desiredValues) values = desiredValues;

	// Common state controls
	if (m_super) {
		if (m_underwater)
			m_currentState = PLAYER_SUPER_UNDERWATER;
		else
			m_currentState = PLAYER_SUPER;
	}
	else {
		if (m_underwater)
			m_currentState = PLAYER_UNDERWATER;
		else {
			if (m_speedshoes)
				m_currentState = PLAYER_SPEEDSHOES;
			else
				m_currentState = PLAYER_DEFAULT;
		}
	}
	// Underwater trigger + transition
	if (m_haswater) {
		if (!m_underwater && (m_position.y >= m_waterHeight)) {
			m_underwater = true;
			m_groundvelocity.x *= 0.5f;
			m_groundvelocity.y *= 0.25f;

			if (abs(m_groundvelocity.y) != 0.0f) {
				soundEmitter->Stop();
				soundEmitter->Play(sfx_05_water);
				m_spawner->Create(FX_SPLASH, vec2(m_position.x, m_waterHeight - 15.0f));
			}
		}
		else if (m_underwater && (m_position.y < m_waterHeight))
		{
			m_underwater = false;
			m_groundvelocity.y *= 2.0f;

			if (abs(m_groundvelocity.y) != 0.0f) {
				soundEmitter->Stop();
				soundEmitter->Play(sfx_05_water);
				m_spawner->Create(FX_SPLASH, vec2(m_position.x, m_waterHeight - 15.0f));
			}
		}
	}





	/* ANIMATION */
	updateAnimation(leftStick.x);


	/* SUPER SONIC: SPARKS */
	// TODO: Also fix Oficina's bug for duplicate stuff.
	if(m_superspark_span > 0) m_superspark_span--;
	if(m_super
		&& abs(m_groundvelocity.x) >= 5.9f
		&& m_superspark_span == 0u)
	{
		m_superspark_span = 15u;
		m_spawner->Create(FX_SPARK, m_position);
	}



	/* DEBUG */
	if (OficinaFramework::InputSystem::PressedKey(SDL_SCANCODE_F4))
		m_viewsensors = !m_viewsensors;

	if (OficinaFramework::ScreenSystem::IsDebugActive())
	{
		// Debug state change
		if (OficinaFramework::InputSystem::PressedKey(SDL_SCANCODE_1))
			m_speedshoes = m_super = false;
		else if (OficinaFramework::InputSystem::PressedKey(SDL_SCANCODE_2))
			m_speedshoes = !m_speedshoes;
		else if (OficinaFramework::InputSystem::PressedKey(SDL_SCANCODE_3))
			m_super = !m_super;

		// Debug repositioning
		if (OficinaFramework::InputSystem::PressingMouse(OficinaFramework::InputSystem::MouseButton::LEFTMB))
		{
			vec2 mousePos = OficinaFramework::InputSystem::GetMousePos().toVec2();
			mousePos /= OficinaFramework::RenderingSystem::GetViewportSize().toVec2();
			mousePos *= OficinaFramework::RenderingSystem::GetResolution().toVec2();
			mousePos += OficinaFramework::RenderingSystem::GetViewportPosition();
			m_position = mousePos;
			m_groundvelocity = vec2::Zero();
			ground = false;
		}

		// Debug info
		std::ostringstream oss;
		oss.clear();
		oss << "POS >> " << m_position << std::endl
			<< "GSP >> " << m_groundvelocity << " @ " << m_angle << "rad" << std::endl
			<< "RSP >> " << m_velocity << std::endl
			<< "VRT >> " << "GROUND " << ground   << " CEILING " << ceiling << std::endl
			<< "HRZ >> " << "LEFT   " << leftwall << " RIGHT   " << rightwall << std::endl
			<< "STA >> ";
		switch (m_currentState) {
		case PLAYER_DEFAULT:          oss << "default";          break;
		case PLAYER_SPEEDSHOES:       oss << "speedshoes";       break;
		case PLAYER_UNDERWATER:       oss << "underwater";       break;
		case PLAYER_SUPER:            oss << "super";            break;
		case PLAYER_SUPER_UNDERWATER: oss << "super_underwater"; break;
		}
		oss << std::endl
			<< "ACN >> ";
		switch (m_currentAction) {
		case PLAYER_NOACTION: oss << "none";          break;
		case PLAYER_JUMPING:  oss << "jumping";       break;
		case PLAYER_ROLLING:  oss << "rolling";       break;
		case PLAYER_SKIDDING: oss << "skidding";      break;
		case PLAYER_CROUCHED: oss << "crouched down"; break;
		case PLAYER_LOOKUP:   oss << "looking up";    break;
		case PLAYER_SPINDASH: oss << "spindashing";   break;
		default:              oss << "action #" << (int)m_currentAction; break;
		}
		oss << std::endl
		    << "SPN >> " << m_spindashRevolutions;
		OficinaFramework::ScreenSystem::Debug_AddLine(oss.str());
	}
}

void Player::updateAnimation(float leftStickX)
{
	// Replace sprite
	if (m_super && m_animator != superAnimator) {
		superAnimator->SetAnimation(sonicAnimator->GetCurrentAnimationName());
		superAnimator->SetAnimationSpeed(sonicAnimator->GetAnimationSpeed());
		m_animator = superAnimator;
	}
	else if (!m_super && m_animator != sonicAnimator) {
		sonicAnimator->SetAnimation(superAnimator->GetCurrentAnimationName());
		sonicAnimator->SetAnimationSpeed(superAnimator->GetAnimationSpeed());
		m_animator = sonicAnimator;
	}

	// Animation name
	if (ground)
	{
		if (m_currentAction == PLAYER_NOACTION)
		{
			//if ((abs(m_groundvelocity.x) < 0.1f)
			//	|| (abs(m_groundvelocity.x) < values->m_unrollMinXSpeed
			if(abs(m_groundvelocity.x) < values->m_accel)
			{
				if ((leftStickX > 0.0f && rightwall)
					|| (leftStickX < 0.0f && leftwall))
					setAnimation("Push");
				else {
					std::string animName = m_animator->GetCurrentAnimationName();
					if (m_animationtimer > 180 && animName == "Idle")
						setAnimation("Idle2");
					else if (animName == "Idle2" && m_animator->GetCurrentFrame() == 3)
						setAnimation("Idle3");
					else if (animName != "Idle2" && animName != "Idle3") {
						m_animationtimer++;
						setAnimation("Idle");
					}
				}
			}
			else
			{
				if (abs(m_groundvelocity.x) >= 9.95f)
				{
					setAnimation("Run2");
					m_animator->SetAnimationSpeed(fmax(0.015f,
						0.01f -
						(0.01f * ((abs(m_groundvelocity.x) - 11.3f) / 4.0f))
					));
				}
				else if (abs(m_groundvelocity.x) >= 5.9f)
				{
					setAnimation("Run");
					m_animator->SetAnimationSpeed(fmax(0.01f,
						0.04f -
						(0.04f * ((abs(m_groundvelocity.x) - 5.9f) / 4.0f))
					));
				}
				else
				{
					setAnimation("Walk");
					m_animator->SetAnimationSpeed(fmax(0.055f,
						0.245f -
						(0.245f * (abs(m_groundvelocity.x) / 4.0f))
					));
				}
			}
		}
		else if (m_currentAction == PLAYER_CROUCHED)
			setAnimation("CrouchDown");
		else if (m_currentAction == PLAYER_LOOKUP)
			setAnimation("LookUp");
		else if (m_currentAction == PLAYER_SPINDASH)
		{
			setAnimation("Roll");
			m_animator->SetAnimationSpeed(0.005f);
		}
		else if (m_currentAction == PLAYER_SKIDDING)
			setAnimation("Skid");
		else if (m_currentAction == PLAYER_ROLLING)
		{
			setAnimation("Roll");
			m_animator->SetAnimationSpeed(fmax(0.002f,
				m_animator->GetDefaultAnimationSpeed() -
				(0.048f * (abs(m_groundvelocity.x) / 4.0f))
			));
		}
	}
	else
	{
		if (m_currentAction == PLAYER_JUMPING)
		{
			setAnimation("Roll");
			m_animator->SetAnimationSpeed(fmax(0.005f,
				m_animator->GetDefaultAnimationSpeed() -
				(0.045f * (abs(m_xSpeedBeforeJump) / 4.0f))
			));
		}
	}

	// Animation direction
	if (m_direction == 1.0f)
		m_animator->SetOrientation(OficinaFramework::RenderingSystem::RENDER_NORMALLY);
	else if(m_direction == -1.0f)
		m_animator->SetOrientation(OficinaFramework::RenderingSystem::RENDER_FLIP_X);

	// Update animation
	m_animator->update();
}

void Player::reset()
{
	m_currentAction = PLAYER_NOACTION;
	m_groundvelocity = 0.0f;
	m_position = vec2(100.0f, 300.0f);
	m_animator->SetAnimation("Idle");
}

void Player::setAnimation(std::string name) {
	if (m_animator->GetCurrentAnimationName() != name) {
		m_animator->SetAnimation(name);
		m_animationtimer = 0;
	}
}

vec2 Player::getGSpeed() {
	return m_groundvelocity;
}

void Player::setGSpeed(vec2 gsp) {
	m_groundvelocity = gsp;
}

PlayerActionState Player::getAction() {
	return m_currentAction;
}

PlayerValueState Player::getState() {
	return m_currentState;
}

void Player::setAction(PlayerActionState st) {
	m_currentAction = st;
}

float Player::getSpindashRev() {
	return m_spindashRevolutions;
}

void Player::setWaterHeight(float f)
{
	m_haswater = true;
	m_waterHeight = f;
}

void Player::disableWater()
{
	m_haswater = false;
}

void Player::setGrid(Grid* grid) {
	m_grid = grid;
}

void Player::disableGrid()  {
	m_grid = nullptr;
}

void Player::setSpawner(EffectSpawner* fxs)
{
	m_spawner = fxs;
}

void Player::LoadContent()
{
	// Sonic
	spriteSheet = new OficinaFramework::RenderingSystem::SpriteSheet(vec2dw(58, 58), vec2b::One(), vec2::Zero());
	spriteSheet->AppendTexture(OficinaFramework::RenderingSystem::TexturePool::LoadTexture("sprites/sonic"));
	sonicAnimator = new OficinaFramework::RenderingSystem::Animation(spriteSheet);

	sonicAnimator->RegisterAnimation(      "Idle", OficinaFramework::RenderingSystem::Animation::AnimationSpecs(0, 0, 1.0f));
	sonicAnimator->RegisterAnimation(     "Idle2", OficinaFramework::RenderingSystem::Animation::AnimationSpecs(1, 3, 0.09f));
	sonicAnimator->RegisterAnimation(     "Idle3", OficinaFramework::RenderingSystem::Animation::AnimationSpecs(3, 4, 3, 0.25f));
	sonicAnimator->RegisterAnimation(      "Walk", OficinaFramework::RenderingSystem::Animation::AnimationSpecs(5, 10, 5, 0.05f));
	sonicAnimator->RegisterAnimation(       "Run", OficinaFramework::RenderingSystem::Animation::AnimationSpecs(11, 14, 11, 0.05f));
	sonicAnimator->RegisterAnimation(      "Roll", OficinaFramework::RenderingSystem::Animation::AnimationSpecs(15, 22, 15, 0.05f));
	sonicAnimator->RegisterAnimation(      "Skid", OficinaFramework::RenderingSystem::Animation::AnimationSpecs(23, 23, 1.0f));
	sonicAnimator->RegisterAnimation(      "Run2", OficinaFramework::RenderingSystem::Animation::AnimationSpecs(24, 27, 24, 0.015f));
	sonicAnimator->RegisterAnimation(      "Push", OficinaFramework::RenderingSystem::Animation::AnimationSpecs(28, 31, 28, 0.5f));
	sonicAnimator->RegisterAnimation("CrouchDown", OficinaFramework::RenderingSystem::Animation::AnimationSpecs(32, 32, 1.0f));
	sonicAnimator->RegisterAnimation(    "LookUp", OficinaFramework::RenderingSystem::Animation::AnimationSpecs(33, 33, 1.0f));
	sonicAnimator->SetSyncToFramerate(true);
	sonicAnimator->SetAnimation("Idle");

	// Super Sonic
	superSheet = new OficinaFramework::RenderingSystem::SpriteSheet(vec2dw(58, 58), vec2b::One(), vec2::Zero());
	superSheet->AppendTexture(OficinaFramework::RenderingSystem::TexturePool::LoadTexture("sprites/supersonic"));
	superAnimator = new OficinaFramework::RenderingSystem::Animation(superSheet);

	superAnimator->RegisterAnimation("Idle", OficinaFramework::RenderingSystem::Animation::AnimationSpecs(0, 0, 1.0f));
	superAnimator->RegisterAnimation("Idle2", OficinaFramework::RenderingSystem::Animation::AnimationSpecs(1, 3, 0.09f));
	superAnimator->RegisterAnimation("Idle3", OficinaFramework::RenderingSystem::Animation::AnimationSpecs(3, 4, 3, 0.25f));
	superAnimator->RegisterAnimation("Walk", OficinaFramework::RenderingSystem::Animation::AnimationSpecs(5, 10, 5, 0.05f));
	superAnimator->RegisterAnimation("Run", OficinaFramework::RenderingSystem::Animation::AnimationSpecs(11, 14, 11, 0.05f));
	superAnimator->RegisterAnimation("Roll", OficinaFramework::RenderingSystem::Animation::AnimationSpecs(15, 22, 15, 0.05f));
	superAnimator->RegisterAnimation("Skid", OficinaFramework::RenderingSystem::Animation::AnimationSpecs(23, 23, 1.0f));
	superAnimator->RegisterAnimation("Run2", OficinaFramework::RenderingSystem::Animation::AnimationSpecs(24, 27, 24, 0.015f));
	superAnimator->RegisterAnimation("Push", OficinaFramework::RenderingSystem::Animation::AnimationSpecs(28, 31, 28, 0.5f));
	superAnimator->RegisterAnimation("CrouchDown", OficinaFramework::RenderingSystem::Animation::AnimationSpecs(32, 32, 1.0f));
	superAnimator->RegisterAnimation("LookUp", OficinaFramework::RenderingSystem::Animation::AnimationSpecs(33, 33, 1.0f));
	superAnimator->SetSyncToFramerate(true);
	superAnimator->SetAnimation("Idle");
	
	// Default animation
	m_animator = m_super ? superAnimator : sonicAnimator;

	sfx_00_skidding =
		OficinaFramework::AudioSystem::AudioPool::LoadAudio("sfx/00_skidding",
			OficinaFramework::AudioSystem::OF_AUDIO_TYPE_OGG);
	sfx_01_rolling =
		OficinaFramework::AudioSystem::AudioPool::LoadAudio("sfx/01_rolling",
			OficinaFramework::AudioSystem::OF_AUDIO_TYPE_OGG);
	sfx_02_jump =
		OficinaFramework::AudioSystem::AudioPool::LoadAudio("sfx/02_jump",
						OficinaFramework::AudioSystem::OF_AUDIO_TYPE_OGG);
	sfx_03_spindash =
		OficinaFramework::AudioSystem::AudioPool::LoadAudio("sfx/03_spindash",
			OficinaFramework::AudioSystem::OF_AUDIO_TYPE_OGG);
	sfx_04_release =
		OficinaFramework::AudioSystem::AudioPool::LoadAudio("sfx/04_release",
			OficinaFramework::AudioSystem::OF_AUDIO_TYPE_OGG);
	sfx_05_water =
		OficinaFramework::AudioSystem::AudioPool::LoadAudio("sfx/05_water",
			OficinaFramework::AudioSystem::OF_AUDIO_TYPE_OGG);
}

void Player::UnloadContent()
{
	sonicAnimator->dispose();
	superAnimator->dispose();
	delete sonicAnimator;
	delete spriteSheet;
	delete superAnimator;
	delete superSheet;
	soundEmitter->Stop();
	OficinaFramework::AudioSystem::AudioPool::UnloadAudio(sfx_00_skidding);
	OficinaFramework::AudioSystem::AudioPool::UnloadAudio(sfx_01_rolling);
	OficinaFramework::AudioSystem::AudioPool::UnloadAudio(sfx_02_jump);
	OficinaFramework::AudioSystem::AudioPool::UnloadAudio(sfx_03_spindash);
	OficinaFramework::AudioSystem::AudioPool::UnloadAudio(sfx_04_release);
	OficinaFramework::AudioSystem::AudioPool::UnloadAudio(sfx_05_water);
	delete soundEmitter;
}

void Player::Draw()
{
	glPushMatrix();
	glTranslatef(m_position.x, m_position.y, 0.0f);
	if (m_currentAction != PLAYER_ROLLING)
		glRotatef(radtodeg(m_angle), 0.0f, 0.0f, 1.0f);
	glTranslatef(0.0f, 1.0f, 0.0f);
	m_animator->Draw(vec2::Zero(), 1.0f, Color4::MaskToColor4(WHITE));
	glPopMatrix();

	if (m_viewsensors)
	{
		glPushMatrix();
		glTranslatef(m_position.x, m_position.y, 0.0f);
		// Linecasts
		// Bottom
		OficinaFramework::RenderingSystem::glColorM(YELLOW, 0.5f);
		glBegin(GL_LINES);
		glVertex2f(-m_vertsensor_dist, 0.0f);
		glVertex2f(-m_vertsensor_dist, m_vlinecast);
		glEnd();
		glBegin(GL_LINES);
		glVertex2f(m_vertsensor_dist, 0.0f);
		glVertex2f(m_vertsensor_dist, m_vlinecast);
		glEnd();

		// Top
		OficinaFramework::RenderingSystem::glColorM(BLUE, 0.5f);
		glBegin(GL_LINES);
		glVertex2f(-m_vertsensor_dist, 0.0f);
		glVertex2f(-m_vertsensor_dist, -m_vlinecast_top);
		glEnd();
		glBegin(GL_LINES);
		glVertex2f(m_vertsensor_dist, 0.0f);
		glVertex2f(m_vertsensor_dist, -m_vlinecast_top);
		glEnd();

		// Left/Right
		OficinaFramework::RenderingSystem::glColorM(RED, 0.5f);
		glBegin(GL_LINES);
		glVertex2f(-m_hlinecast, 4.0f);
		glVertex2f(m_hlinecast, 4.0f);
		glEnd();

		// AABB
		OficinaFramework::RenderingSystem::glColorM(BLACK, 0.3f);
		glBegin(GL_LINE_LOOP);
		glVertex2f(-32.0f, -m_vlinecast);
		glVertex2f(32.0f, -m_vlinecast);
		glVertex2f(32.0f, m_vlinecast);
		glVertex2f(-32.0f, m_vlinecast);
		glEnd();
		glPopMatrix();

		// Cell highlighting
		if (m_grid) {
			vec2 cellSize = m_grid->getCellSize();
			vec2 cellPosition(floor((m_position.x / cellSize.x)) * cellSize.x,
				floor((m_position.y / cellSize.y)) * cellSize.y);

			/*OficinaFramework::RenderingSystem::glColorM(GREEN, 0.3f);

			// Top
			glPushMatrix();
			glTranslatef(cellPosition.x, cellPosition.y - cellSize.y, 0.0f);
			glBegin(GL_LINE_LOOP);
			glVertex2f(0.0f, 0.0f);
			glVertex2f(cellSize.x, 0.0f);
			glVertex2f(cellSize.x, cellSize.y);
			glVertex2f(0.0f, cellSize.y);
			glEnd();
			glPopMatrix();

			// Bottom
			glPushMatrix();
			glTranslatef(cellPosition.x, cellPosition.y + cellSize.y, 0.0f);
			glBegin(GL_LINE_LOOP);
			glVertex2f(0.0f, 0.0f);
			glVertex2f(cellSize.x, 0.0f);
			glVertex2f(cellSize.x, cellSize.y);
			glVertex2f(0.0f, cellSize.y);
			glEnd();
			glPopMatrix();

			// Left
			glPushMatrix();
			glTranslatef(cellPosition.x - cellSize.x, cellPosition.y, 0.0f);
			glBegin(GL_LINE_LOOP);
			glVertex2f(0.0f, 0.0f);
			glVertex2f(cellSize.x, 0.0f);
			glVertex2f(cellSize.x, cellSize.y);
			glVertex2f(0.0f, cellSize.y);
			glEnd();
			glPopMatrix();

			// Right
			glPushMatrix();
			glTranslatef(cellPosition.x + cellSize.x, cellPosition.y, 0.0f);
			glBegin(GL_LINE_LOOP);
			glVertex2f(0.0f, 0.0f);
			glVertex2f(cellSize.x, 0.0f);
			glVertex2f(cellSize.x, cellSize.y);
			glVertex2f(0.0f, cellSize.y);
			glEnd();
			glPopMatrix();

			// LeftTop
			glPushMatrix();
			glTranslatef(cellPosition.x - cellSize.x, cellPosition.y - cellSize.y, 0.0f);
			glBegin(GL_LINE_LOOP);
			glVertex2f(0.0f, 0.0f);
			glVertex2f(cellSize.x, 0.0f);
			glVertex2f(cellSize.x, cellSize.y);
			glVertex2f(0.0f, cellSize.y);
			glEnd();
			glPopMatrix();

			// RightTop
			glPushMatrix();
			glTranslatef(cellPosition.x + cellSize.x, cellPosition.y - cellSize.y, 0.0f);
			glBegin(GL_LINE_LOOP);
			glVertex2f(0.0f, 0.0f);
			glVertex2f(cellSize.x, 0.0f);
			glVertex2f(cellSize.x, cellSize.y);
			glVertex2f(0.0f, cellSize.y);
			glEnd();
			glPopMatrix();

			// LeftBottom
			glPushMatrix();
			glTranslatef(cellPosition.x - cellSize.x, cellPosition.y + cellSize.y, 0.0f);
			glBegin(GL_LINE_LOOP);
			glVertex2f(0.0f, 0.0f);
			glVertex2f(cellSize.x, 0.0f);
			glVertex2f(cellSize.x, cellSize.y);
			glVertex2f(0.0f, cellSize.y);
			glEnd();
			glPopMatrix();

			// RightBottom
			glPushMatrix();
			glTranslatef(cellPosition.x + cellSize.x, cellPosition.y + cellSize.y, 0.0f);
			glBegin(GL_LINE_LOOP);
			glVertex2f(0.0f, 0.0f);
			glVertex2f(cellSize.x, 0.0f);
			glVertex2f(cellSize.x, cellSize.y);
			glVertex2f(0.0f, cellSize.y);
			glEnd();
			glPopMatrix();*/

			// Central
			OficinaFramework::RenderingSystem::glColorM(RED, 1.0f);
			glPushMatrix();
			glTranslatef(cellPosition.x, cellPosition.y, 0.0f);
			glBegin(GL_LINE_LOOP);
			glVertex2f(0.0f, 0.0f);
			glVertex2f(cellSize.x, 0.0f);
			glVertex2f(cellSize.x, cellSize.y);
			glVertex2f(0.0f, cellSize.y);
			glEnd();
			glPopMatrix();
		}
	}
}


