#include "Player.hpp"
#include "Solid.hpp"
#include "Grid.hpp"
#include <sstream>
#include <optional>

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
	m_lastCheckpointPos = m_position;
	ground = true;
	pressingJump = false;
	jumpAngle = 0.0f;
	values = &default_values;
	m_currentState = PLAYER_DEFAULT;
	soundEmitter = new OficinaFramework::AudioSystem::AudioSource;
	m_currentAction = PLAYER_NOACTION;
	m_direction = 1.0f;
	m_spindashRevolutions = 0.0f;
	m_super = false;
	m_underwater = false;
	m_speedshoes = false;
	m_haswater = false;
	m_viewsensors = true;
	m_grid = nullptr;
	m_currentshield = SHIELD_NONE;
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
	// Nor when dead...
	if (m_currentAction != PLAYER_ROLLING
		&& m_currentAction != PLAYER_CROUCHED
		&& m_currentAction != PLAYER_LOOKUP
		&& m_currentAction != PLAYER_SPINDASH
		&& m_currentAction != PLAYER_DEATH
		&& m_currentAction != PLAYER_DROWN)
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
		if (ground && ((leftStick.x == 0.0f) || m_currentAction != PLAYER_NOACTION))
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
					soundEmitter->Play(sfx.s00_skidding);
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
					soundEmitter->Play(sfx.s00_skidding);
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
	// Only works if not looking up, crouching, spindashing nor dead
	if (m_currentAction != PLAYER_CROUCHED
	&& m_currentAction != PLAYER_LOOKUP
	&& m_currentAction != PLAYER_SPINDASH
	&& m_currentAction != PLAYER_DEATH
	&& m_currentAction != PLAYER_DROWN)
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

        std::optional<vec2> bottomInterceptL = std::nullopt,
            bottomInterceptR = std::nullopt;
		
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
	
	// Collision detection only works if Sonic is alive!
	if(m_currentAction != PLAYER_DEATH
	&& m_currentAction != PLAYER_DROWN)
	{
            // Level collision
            if(m_lvl != nullptr) {
                // Get collision surrounding our character
                std::vector<CollisionArray*> collision =
                    m_lvl->getSurroundingCollision(m_position);

                auto correctPosition = [&](int region, vec2 pos, vec2& correction) {
                    // setup correction as if it were the center
                    // tile
                    correction = vec2(std::trunc(m_position.x / 128.0) * 128.0,
                                      std::trunc(m_position.y / 128.0) * 128.0);
                    
                    switch(region) {
                        // Upper quadrants
                    case 0:
                        correction.x -= 128.0;
                        correction.y -= 128.0;
                        break;
                    case 1:
                        correction.y -= 128.0;
                        break;
                    case 2:
                        correction.x += 128.0;
                        correction.y -= 128.0;
                        break;

                        // Middle line
                    case 3:
                        correction.x -= 128.0;
                        break;
                    case 4: break;
                    case 5:
                        correction.x += 128.0;
                        break;

                        // Lower quadrants
                    case 6:
                        correction.x -= 128.0;
                        correction.y += 128.0;
                        break;
                    case 7:
                        correction.y += 128.0;
                        break;
                    case 8:
                        correction.x += 128.0;
                        correction.y += 128.0;
                        break;
                    }
                    
                    return pos - correction;
                };

                for(int i = 0; i < 9; i++) {
                    if(collision[i] != nullptr) {
                        for(size_t j = 0; j < (collision[i])->size(); j++) {
                            CollisionShape *shape = (*(collision[i]))[j];
                            // Linecast for each sensor.
                            // Also, correct sensor position before linecast
                            vec2 correction;
                            
                            auto lsensor = correctPosition(i, m_leftSensor, correction);
                            if(auto interceptL = shape->Linecast(lsensor, vec2::Left(), m_hlinecast)) {
                                auto collision_point = interceptL.value();
                                collision_point += correction;
                                if(m_direction < 0.0f) leftwall = true;
                                if(m_groundvelocity.x < 0.0f && m_position.x - m_hlinecast < collision_point.x) {
                                    m_groundvelocity.x = 0.0f;
                                    m_position.x = collision_point.x + m_hlinecast;
                                    continue;
                                }
                            }

                            auto rsensor = correctPosition(i, m_rightSensor, correction);
                            if(auto interceptR = shape->Linecast(rsensor, vec2::Right(), m_hlinecast)) {
                                auto collision_point = interceptR.value();
                                collision_point += correction;
                                if(m_direction > 0.0f) rightwall = true;
                                if(m_groundvelocity.x > 0.0f && m_position.x + m_hlinecast > collision_point.x) {
                                    m_groundvelocity.x = 0.0f;
                                    m_position.x = collision_point.x - m_hlinecast;
                                    continue;
                                }
                            }

                            auto bsensorl = correctPosition(i, m_bottomSensorL, correction);
                            if(m_groundvelocity.y >= 0.0f) {
                                if (auto interceptBL = shape->Linecast(bsensorl, vec2::Down(), 16)) {
                                    auto collision_point = interceptBL.value();
                                    collision_point += correction;
                                    if ((m_position.y + m_hitboxRadius > collision_point.y) || ground == true)
                                    {
                                        std::cout << "found ground (L)" << std::endl;
					foundGround = true;
                                        bottomInterceptL = std::nullopt;
					if (!bottomInterceptL || collision_point.y <= bottomInterceptL.value().y)
                                            bottomInterceptL = collision_point;
                                    }
                                }
                            }

                            auto bsensorr = correctPosition(i, m_bottomSensorR, correction);
                            if(m_groundvelocity.y >= 0.0f) {
                                if (auto interceptBR = shape->Linecast(bsensorr, vec2::Down(), 16)) {
                                    auto collision_point = interceptBR.value();
                                    collision_point += correction;
                                    if ((m_position.y + m_hitboxRadius > collision_point.y) || ground == true)
                                    {
                                        std::cout << "found ground (R)" << std::endl;
					foundGround = true;
                                        bottomInterceptR = std::nullopt;
				        if (!bottomInterceptR || collision_point.y <= bottomInterceptR.value().y)
                                            bottomInterceptR = collision_point;
                                    }
                                }
                            }

                            //     auto rsensor = correctPosition(i, m_rightSensor);
                            //     auto bsensorl = correctPosition(i, m_bottomSensorL);
                            //     auto bsensorr = correctPosition(i, m_bottomSensorR);
                            //     auto tsensorl = correctPosition(i, m_topSensorL);
                            // auto tsensorr = correctPosition(i, m_topSensorR);
                            
                        }
                    }
                }
            }

            // Object collision
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

                // Use proper map tile collision
                if (bottomInterceptL && bottomInterceptR) {
                    std::cout << "some bottom intercept" << std::endl;
                    float lTop = bottomInterceptL.value().y,
                        rTop = bottomInterceptR.value().y;
                    if (lTop <= rTop) {
                        biggestHeight = lTop;
                        // Get angular coefficient of L here
                        biggestsAngle = 0.0f; // TODO
                    }
                    else {
                        biggestHeight = rTop;
                        // Get angular coefficient of R here
                        biggestsAngle = 0.0f; // TODO
                    }
                }
                else {
                    if (bottomInterceptL) {
                        std::cout << "left intercept" << std::endl;
                        biggestHeight = bottomInterceptL.value().y;
                        // Get angular coefficient of L here
                        biggestsAngle = 0.0f; // TODO
                    }
                    if (bottomInterceptR) {
                        std::cout << "right intercept" << std::endl;
                        biggestHeight = bottomInterceptR.value().y;
                        // Get angular coefficient of R here
                        biggestsAngle = 0.0f; // TODO
                    }
                }

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
		//m_position.y = biggestHeight - m_hitboxRadius;
                m_position.y = biggestHeight;
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
		soundEmitter->Play(sfx.s02_jump);
		m_currentAction = PLAYER_JUMPING;
		ground = false;
		jumpAngle = m_angle;
		m_xSpeedBeforeJump = m_groundvelocity.x;
		m_groundvelocity.x += values->m_jump_strength * -sinf(jumpAngle);
		m_groundvelocity.y += values->m_jump_strength *  cosf(jumpAngle);
	}
	// Minimal jump
	if (!ground && !pressingJump && m_groundvelocity.y < values->m_min_jump_str
		// Of course, if you're using a death animation, this has no effect.
		&& m_currentAction != PLAYER_DEATH
		&& m_currentAction != PLAYER_DROWN)
		m_groundvelocity.y = values->m_min_jump_str * cosf(jumpAngle);

	// Rolling
	if(ground
		&& m_currentAction != PLAYER_ROLLING
		&& (abs(m_groundvelocity.x) >= values->m_rollingMinXSpeed)
		&& leftStick.y > 0.0f) {
		soundEmitter->Stop();
		soundEmitter->Play(sfx.s01_rolling);
		m_currentAction = PLAYER_ROLLING;
	}
	// If on air and rolling, then you're technically jumping.
	// This will also allow you to regain control.
	if (!ground && m_currentAction == PLAYER_ROLLING)
		m_currentAction = PLAYER_JUMPING;

	// Crouch down & Look up
	if (ground
		&& (abs(m_groundvelocity.x) < values->m_rollingMinXSpeed)
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
		if(leftStick.y == 0.0f && (abs(m_groundvelocity.x) < values->m_rollingMinXSpeed))
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
			soundEmitter->Play(sfx.s03_spindash);
		}
		else if (m_currentAction == PLAYER_SPINDASH)
		{
			// If pressed A, then rev
			if (OficinaFramework::InputSystem::PressedButton(OficinaFramework::InputSystem::GamePadButton::A))
			{
				m_spindashRevolutions += 2.0f;
				soundEmitter->Stop();
				soundEmitter->Play(sfx.s03_spindash);
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
				soundEmitter->Play(sfx.s04_release);
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
	// Only works if Sonic ain't dead
	if(m_currentAction != PLAYER_DEATH
	&& m_currentAction != PLAYER_DROWN)
	{
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
	}
	// Underwater trigger + transition
	if (m_haswater
	&& m_currentAction != PLAYER_DEATH
	&& m_currentAction != PLAYER_DROWN)
	{
		if (!m_underwater && (m_position.y >= m_waterHeight))
		{
			m_underwater = true;
			m_groundvelocity.x *= 0.5f;
			m_groundvelocity.y *= 0.25f;

			if (abs(m_groundvelocity.y) != 0.0f) {
				soundEmitter->Stop();
				soundEmitter->Play(sfx.s05_water);
				m_spawner->Create(FX_SPLASH, vec2(m_position.x, m_waterHeight - 15.0f));
			}
		}
		else if (m_underwater)
		{
			// Underwater-yet check
			if (m_position.y < m_waterHeight)
			{
				m_underwater = false;
				m_groundvelocity.y *= 2.0f;

				if (abs(m_groundvelocity.y) != 0.0f) {
					soundEmitter->Stop();
					soundEmitter->Play(sfx.s05_water);
					m_spawner->Create(FX_SPLASH, vec2(m_position.x, m_waterHeight - 15.0f));
				}
			}

			// Bubble spawn check
			if(m_currentshield != SHIELD_BUBBLE
			&& m_currentAction != PLAYER_DEATH
			&& m_currentAction != PLAYER_DROWN)
			{
				if (m_minibubble_span == 0u)
				{
					m_spawner->Create(FX_SMALLBUBBLE, m_position, 1.0f,
						[&](vec2& Position, vec2& Velocity, bool& Destroy)
						{
							if(Velocity.y == 0.0f)
								Velocity.y = -0.5f;
							Position += Velocity;
							if (Position.y - 4.0f <= m_waterHeight)
								Destroy = true;
						}
					);
					m_minibubble_span = 120u;
				}
				else m_minibubble_span--;
			}

			// Drowning-related stuff

			// Do not drown if on bubble shield
			if(m_currentshield == SHIELD_BUBBLE)
				m_drown_span = 1800u;
			else if(m_drown_span > 0u)
				m_drown_span--;

			// SFX plays for 5s, 10s and 15s
			if(m_drown_span == 1500u
				|| m_drown_span == 1200u
				|| m_drown_span == 900u)
			{
				soundEmitter->Stop();
				soundEmitter->Play(sfx.s08_watercount);
			}
			// Jingle starts at 18s, takes 16s to play
			//if(m_drown_span == 720u) {}
			// TODO: Kill Sonic on zero
		}
		// Prevent drowning
		else
		{
			m_drown_span = 1800u;
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
		m_spawner->Create(FX_SPARK, m_position, 0.7f);
	}

	/* SUPER SONIC: SHIELDS */
	// Super Sonic shows no shields
	if(m_super && m_shieldhandle)
	{
		m_shieldhandle->RemoveMe();
		m_shieldhandle = nullptr;
	}
	// Also, if Super Sonic acquires shields and loses
	// its Super State, then the shield is now available.
	else if(m_currentshield != SHIELD_NONE
			&& !m_super && !m_shieldhandle)
	{
		switch(m_currentshield)
		{
		default: break;
		case SHIELD_NORMAL:
			m_shieldhandle = m_spawner->Create(FX_NORMALSHIELD, this, vec2(0.0f, 2.0f), 0.45f);
			break;
		case SHIELD_BUBBLE:
			m_shieldhandle = m_spawner->Create(FX_BUBBLESHIELD, this);
			break;
		}
	}



	/* DEBUG */
	if (OficinaFramework::InputSystem::PressedKey(SDL_SCANCODE_4)
		|| OficinaFramework::InputSystem::PressedButton(OficinaFramework::InputSystem::GamePadButton::HAT_LEFT))
		m_viewsensors = !m_viewsensors;

	if (OficinaFramework::ScreenSystem::IsDebugActive())
	{
		// Debug state change
		if (OficinaFramework::InputSystem::PressedKey(SDL_SCANCODE_1)
			|| OficinaFramework::InputSystem::PressedButton(OficinaFramework::InputSystem::GamePadButton::HAT_UP))
			m_speedshoes = m_super = false;
		else if (OficinaFramework::InputSystem::PressedKey(SDL_SCANCODE_2)
				|| OficinaFramework::InputSystem::PressedButton(OficinaFramework::InputSystem::GamePadButton::HAT_RIGHT))
			m_speedshoes = !m_speedshoes;
		else if (OficinaFramework::InputSystem::PressedKey(SDL_SCANCODE_3)
				|| OficinaFramework::InputSystem::PressedButton(OficinaFramework::InputSystem::GamePadButton::HAT_DOWN))
			m_super = !m_super;
		else if(OficinaFramework::InputSystem::PressedKey(SDL_SCANCODE_7))
			Kill();
		else if(OficinaFramework::InputSystem::PressedKey(SDL_SCANCODE_8))
			Kill(DEATH_DROWN);

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

		// Debug shield
		if(OficinaFramework::InputSystem::PressedKey(SDL_SCANCODE_5)
			|| OficinaFramework::InputSystem::PressedButton(OficinaFramework::InputSystem::GamePadButton::X))
		{
			if(m_currentshield == SHIELD_NORMAL)
			{
				if(m_shieldhandle)
				{
					m_shieldhandle->RemoveMe();
					m_shieldhandle = nullptr;
				}
				m_currentshield = SHIELD_NONE;
			}
			else
			{
				if(m_currentshield != SHIELD_NONE && m_shieldhandle)
					m_shieldhandle->RemoveMe();
				m_currentshield = SHIELD_NORMAL;
				if(!m_super)
					m_shieldhandle = m_spawner->Create(FX_NORMALSHIELD, this, vec2(0.0f, 2.0f), 0.45f);
				soundEmitter->Stop();
				soundEmitter->Play(sfx.s06_normalshield);
			}
		}
		else if(OficinaFramework::InputSystem::PressedKey(SDL_SCANCODE_6)
			|| OficinaFramework::InputSystem::PressedButton(OficinaFramework::InputSystem::GamePadButton::Y))
		{
			if(m_currentshield == SHIELD_BUBBLE)
			{
				if(m_shieldhandle)
				{
					m_shieldhandle->RemoveMe();
					m_shieldhandle = nullptr;
				}
				m_currentshield = SHIELD_NONE;
			}
			else
			{
				if(m_currentshield != SHIELD_NONE && m_shieldhandle)
					m_shieldhandle->RemoveMe();
				m_currentshield = SHIELD_BUBBLE;
				if(!m_super)
					m_shieldhandle = m_spawner->Create(FX_BUBBLESHIELD, this);
				soundEmitter->Stop();
				soundEmitter->Play(sfx.s07_bubbleshield);
			}
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
		case PLAYER_DEATH:    oss << "dead";          break;
		case PLAYER_DROWN:    oss << "drowned";       break;
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
		else if(m_currentAction == PLAYER_DEATH)  setAnimation("Death");
		else if(m_currentAction == PLAYER_DROWN)  setAnimation("Death"); // TODO: Drowning animation
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
	m_position = m_lastCheckpointPos;
	m_animator->SetAnimation("Idle");
	m_currentshield = SHIELD_NONE;
	if(m_shieldhandle)
	{
		m_shieldhandle->RemoveMe();
		m_shieldhandle = nullptr;
	}
	m_super      = false;
	m_speedshoes = false;
	m_direction  = 1.0f;
	m_drown_span = 1800u;
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

bool* Player::getViewSensorsPtr() {
	return &m_viewsensors;
}

word Player::getDrownSpan() {
	return m_drown_span;
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

void Player::setLevel(Level* lvl)
{
    this->m_lvl = lvl;
}

void Player::Kill(DeathType t)
{
	// Never give Sonic an endless death. Really.
	if(m_currentAction == PLAYER_DEATH
	|| m_currentAction == PLAYER_DROWN)
		return;

	ground = false;
	m_groundvelocity.x = 0.0f;
	m_angle = 0.0f;
	m_direction = 1.0f;
	
	// Disable shield
	if(m_shieldhandle) {
		m_shieldhandle->RemoveMe();
		m_shieldhandle = nullptr;
	}
	m_currentshield = SHIELD_NONE;
	soundEmitter->Stop();

	// Change actions; and also, change variables if drowned
	switch(t)
	{
	case DEATH_DROWN:
		m_currentAction = PLAYER_DROWN;
		m_currentState = (m_super ? PLAYER_SUPER_UNDERWATER : PLAYER_UNDERWATER);
		soundEmitter->Play(sfx.s0A_drown);
		m_groundvelocity.y = 0.0f;
		break;

	default:
		m_currentAction = PLAYER_DEATH;
		m_currentState = (m_super ? PLAYER_SUPER : PLAYER_DEFAULT);
		soundEmitter->Play(sfx.s0F_death);
		m_groundvelocity.y = -6.5f;
		break;
	};
}

void Player::LoadContent()
{
	// Sonic
	t_sonic = OficinaFramework::RenderingSystem::TexturePool::LoadTexture("sprites/sonic");
	spriteSheet = new OficinaFramework::RenderingSystem::SpriteSheet(vec2dw(58, 58), vec2b::One(), vec2::Zero());
	spriteSheet->AppendTexture(t_sonic);
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
	sonicAnimator->RegisterAnimation(    "Death",  OficinaFramework::RenderingSystem::Animation::AnimationSpecs(34, 34, 1.0f));
	sonicAnimator->SetSyncToFramerate(true);
	sonicAnimator->SetAnimation("Idle");

	// Super Sonic
	t_supersonic = OficinaFramework::RenderingSystem::TexturePool::LoadTexture("sprites/supersonic");
	superSheet = new OficinaFramework::RenderingSystem::SpriteSheet(vec2dw(58, 58), vec2b::One(), vec2::Zero());
	superSheet->AppendTexture(t_supersonic);
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
	superAnimator->RegisterAnimation("Death",  OficinaFramework::RenderingSystem::Animation::AnimationSpecs(34, 34, 1.0f));
	superAnimator->SetSyncToFramerate(true);
	superAnimator->SetAnimation("Idle");
	
	// Default animation
	m_animator = m_super ? superAnimator : sonicAnimator;

	sfx.s00_skidding =
		OficinaFramework::AudioSystem::AudioPool::LoadAudio("sfx/00_skidding",
			OficinaFramework::AudioSystem::OF_AUDIO_TYPE_OGG);
	sfx.s01_rolling =
		OficinaFramework::AudioSystem::AudioPool::LoadAudio("sfx/01_rolling",
			OficinaFramework::AudioSystem::OF_AUDIO_TYPE_OGG);
	sfx.s02_jump =
		OficinaFramework::AudioSystem::AudioPool::LoadAudio("sfx/02_jump",
						OficinaFramework::AudioSystem::OF_AUDIO_TYPE_OGG);
	sfx.s03_spindash =
		OficinaFramework::AudioSystem::AudioPool::LoadAudio("sfx/03_spindash",
			OficinaFramework::AudioSystem::OF_AUDIO_TYPE_OGG);
	sfx.s04_release =
		OficinaFramework::AudioSystem::AudioPool::LoadAudio("sfx/04_release",
			OficinaFramework::AudioSystem::OF_AUDIO_TYPE_OGG);
	sfx.s05_water =
		OficinaFramework::AudioSystem::AudioPool::LoadAudio("sfx/05_water",
			OficinaFramework::AudioSystem::OF_AUDIO_TYPE_OGG);
	sfx.s06_normalshield =
		OficinaFramework::AudioSystem::AudioPool::LoadAudio("sfx/06_normalshield",
			OficinaFramework::AudioSystem::OF_AUDIO_TYPE_OGG);
	sfx.s07_bubbleshield =
		OficinaFramework::AudioSystem::AudioPool::LoadAudio("sfx/07_bubbleshield",
			OficinaFramework::AudioSystem::OF_AUDIO_TYPE_OGG);
	sfx.s08_watercount =
		OficinaFramework::AudioSystem::AudioPool::LoadAudio("sfx/08_watercount",
			OficinaFramework::AudioSystem::OF_AUDIO_TYPE_OGG);
	//--
	sfx.s0A_drown =
		OficinaFramework::AudioSystem::AudioPool::LoadAudio("sfx/0A_drown",
			OficinaFramework::AudioSystem::OF_AUDIO_TYPE_OGG);
	//--
	sfx.s0F_death =
		OficinaFramework::AudioSystem::AudioPool::LoadAudio("sfx/0F_death",
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
	OficinaFramework::RenderingSystem::TexturePool::DisposeTexture(t_sonic);
	OficinaFramework::RenderingSystem::TexturePool::DisposeTexture(t_supersonic);
	OficinaFramework::AudioSystem::AudioPool::UnloadAudio(sfx.s00_skidding);
	OficinaFramework::AudioSystem::AudioPool::UnloadAudio(sfx.s01_rolling);
	OficinaFramework::AudioSystem::AudioPool::UnloadAudio(sfx.s02_jump);
	OficinaFramework::AudioSystem::AudioPool::UnloadAudio(sfx.s03_spindash);
	OficinaFramework::AudioSystem::AudioPool::UnloadAudio(sfx.s04_release);
	OficinaFramework::AudioSystem::AudioPool::UnloadAudio(sfx.s05_water);
	OficinaFramework::AudioSystem::AudioPool::UnloadAudio(sfx.s06_normalshield);
	OficinaFramework::AudioSystem::AudioPool::UnloadAudio(sfx.s07_bubbleshield);
	OficinaFramework::AudioSystem::AudioPool::UnloadAudio(sfx.s08_watercount);
	OficinaFramework::AudioSystem::AudioPool::UnloadAudio(sfx.s0A_drown);
	OficinaFramework::AudioSystem::AudioPool::UnloadAudio(sfx.s0F_death);
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
		// if (m_grid) {
		// 	vec2 cellSize = m_grid->getCellSize();
		// 	vec2 cellPosition(floor((m_position.x / cellSize.x)) * cellSize.x,
		// 		floor((m_position.y / cellSize.y)) * cellSize.y);

		// 	OficinaFramework::RenderingSystem::glColorM(GREEN, 0.3f);

		// 	// Top
		// 	glPushMatrix();
		// 	glTranslatef(cellPosition.x, cellPosition.y - cellSize.y, 0.0f);
		// 	glBegin(GL_LINE_LOOP);
		// 	glVertex2f(0.0f, 0.0f);
		// 	glVertex2f(cellSize.x, 0.0f);
		// 	glVertex2f(cellSize.x, cellSize.y);
		// 	glVertex2f(0.0f, cellSize.y);
		// 	glEnd();
		// 	glPopMatrix();

		// 	// Bottom
		// 	glPushMatrix();
		// 	glTranslatef(cellPosition.x, cellPosition.y + cellSize.y, 0.0f);
		// 	glBegin(GL_LINE_LOOP);
		// 	glVertex2f(0.0f, 0.0f);
		// 	glVertex2f(cellSize.x, 0.0f);
		// 	glVertex2f(cellSize.x, cellSize.y);
		// 	glVertex2f(0.0f, cellSize.y);
		// 	glEnd();
		// 	glPopMatrix();

		// 	// Left
		// 	glPushMatrix();
		// 	glTranslatef(cellPosition.x - cellSize.x, cellPosition.y, 0.0f);
		// 	glBegin(GL_LINE_LOOP);
		// 	glVertex2f(0.0f, 0.0f);
		// 	glVertex2f(cellSize.x, 0.0f);
		// 	glVertex2f(cellSize.x, cellSize.y);
		// 	glVertex2f(0.0f, cellSize.y);
		// 	glEnd();
		// 	glPopMatrix();

		// 	// Right
		// 	glPushMatrix();
		// 	glTranslatef(cellPosition.x + cellSize.x, cellPosition.y, 0.0f);
		// 	glBegin(GL_LINE_LOOP);
		// 	glVertex2f(0.0f, 0.0f);
		// 	glVertex2f(cellSize.x, 0.0f);
		// 	glVertex2f(cellSize.x, cellSize.y);
		// 	glVertex2f(0.0f, cellSize.y);
		// 	glEnd();
		// 	glPopMatrix();

		// 	// LeftTop
		// 	glPushMatrix();
		// 	glTranslatef(cellPosition.x - cellSize.x, cellPosition.y - cellSize.y, 0.0f);
		// 	glBegin(GL_LINE_LOOP);
		// 	glVertex2f(0.0f, 0.0f);
		// 	glVertex2f(cellSize.x, 0.0f);
		// 	glVertex2f(cellSize.x, cellSize.y);
		// 	glVertex2f(0.0f, cellSize.y);
		// 	glEnd();
		// 	glPopMatrix();

		// 	// RightTop
		// 	glPushMatrix();
		// 	glTranslatef(cellPosition.x + cellSize.x, cellPosition.y - cellSize.y, 0.0f);
		// 	glBegin(GL_LINE_LOOP);
		// 	glVertex2f(0.0f, 0.0f);
		// 	glVertex2f(cellSize.x, 0.0f);
		// 	glVertex2f(cellSize.x, cellSize.y);
		// 	glVertex2f(0.0f, cellSize.y);
		// 	glEnd();
		// 	glPopMatrix();

		// 	// LeftBottom
		// 	glPushMatrix();
		// 	glTranslatef(cellPosition.x - cellSize.x, cellPosition.y + cellSize.y, 0.0f);
		// 	glBegin(GL_LINE_LOOP);
		// 	glVertex2f(0.0f, 0.0f);
		// 	glVertex2f(cellSize.x, 0.0f);
		// 	glVertex2f(cellSize.x, cellSize.y);
		// 	glVertex2f(0.0f, cellSize.y);
		// 	glEnd();
		// 	glPopMatrix();

		// 	// RightBottom
		// 	glPushMatrix();
		// 	glTranslatef(cellPosition.x + cellSize.x, cellPosition.y + cellSize.y, 0.0f);
		// 	glBegin(GL_LINE_LOOP);
		// 	glVertex2f(0.0f, 0.0f);
		// 	glVertex2f(cellSize.x, 0.0f);
		// 	glVertex2f(cellSize.x, cellSize.y);
		// 	glVertex2f(0.0f, cellSize.y);
		// 	glEnd();
		// 	glPopMatrix();

		// 	// Central
		// 	OficinaFramework::RenderingSystem::glColorM(RED, 1.0f);
		// 	glPushMatrix();
		// 	glTranslatef(cellPosition.x, cellPosition.y, 0.0f);
		// 	glBegin(GL_LINE_LOOP);
		// 	glVertex2f(0.0f, 0.0f);
		// 	glVertex2f(cellSize.x, 0.0f);
		// 	glVertex2f(cellSize.x, cellSize.y);
		// 	glVertex2f(0.0f, cellSize.y);
		// 	glEnd();
		// 	glPopMatrix();
		// }
	}
}


