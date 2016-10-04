#pragma once
#include <OficinaFramework/OficinaTypes.hpp>
#include <OficinaFramework/EntitySystem.hpp>
#include <vector>

typedef OficinaFramework::RenderingSystem::Texture ofTexture;
typedef OficinaFramework::RenderingSystem::TexturePool ofTexturePool;
typedef OficinaFramework::EntitySystem::DrawableEntity ofDrawableEntity;

// TO-DO!
/*enum EWColliderType
{
    EWCT_HURTBOX,
    EWCT_HITBOX,
    EWCT_NEUTRAL
};

enum EWColliderBox
{
    EWCB_OVAL,
    EWCB_BOX
};

class EWCollider
    : public ofDrawableEntity
{
private:
    EWColliderType m_type;
    EWColliderBox  m_boxformat;
    float          m_boxrelst = 0.0f,
                   m_boxrelfn = 0.0f;
public:
    // Inherited via DrawableEntity
    virtual void Initialize() override;
    virtual void Update() override;
    virtual void LoadContent() override;
    virtual void UnloadContent() override;
    virtual void Draw() override;
};*/




class EWJoint
{
public:
    struct State
    {
        vec2  position;
        float angle;

        State(vec2 pos, float a) : position(pos), angle(a) {}
    };
private:
    float m_maxAngle = 360.0f;
    float m_minAngle = 0.0f;

    EWJoint* parent = nullptr;
    EWJoint::State defaultState = {vec2::Zero(), 0.0f};
    EWJoint::State currentState = {vec2::Zero(), 0.0f};
public:
    EWJoint();
    EWJoint(EWJoint::State);

    void setPosition(vec2);
    void setParent(EWJoint*);
    void setAngle(float);
    void setMaxAngle(float);
    void setMinAngle(float);

    vec2 getPosition();
    EWJoint* getParent();
    float getAngle();
    float getMaxAngle();
    float getMinAngle();

    void reset();
    void parentTransform();
    void translateToMe();
    void rotateToMe();

    void update();
    void draw();
};

class EggWalker
    : public ofDrawableEntity
{
private:
    EWJoint* m_jMain;

    EWJoint* m_jTorso,
           * m_jHead,
           * m_jBForearm,
           * m_jBArm,
           * m_jFForearm,
           * m_jFArm,
           * m_jBThigh,
           * m_jFThigh,
           * m_jBLeg,
           * m_jFLeg;
    std::vector<EWJoint*> m_joints;

    ofTexture* m_tHead,
             * m_tTorso,
             * m_tRocket,
             * m_tThigh,
             * m_tLeg,
             * m_tForearm,
             * m_tArm,
             * m_tShoulder;

public:
    EggWalker();
    virtual ~EggWalker();

    void drawPiece(ofTexture*, EWJoint*, vec2 hotspot);

    // Inherited via DrawableEntity
    virtual void Initialize() override;
    virtual void Update() override;
    virtual void LoadContent() override;
    virtual void UnloadContent() override;
    virtual void Draw() override;
};