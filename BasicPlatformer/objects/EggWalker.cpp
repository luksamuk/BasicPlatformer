#include "EggWalker.hpp"
#include "../OFSDebugUtil.hpp"
using namespace OFSDebugUtil;

// EWJoint Stuff
EWJoint::EWJoint() {}
EWJoint::EWJoint(EWJoint::State st) {
    defaultState = st;
    currentState = defaultState;
}

void EWJoint::setPosition(vec2 pos) {
    currentState.position = pos;
}
vec2 EWJoint::getPosition() {
    return currentState.position;
}
void EWJoint::setParent(EWJoint* j) {
    parent = j;
}
EWJoint* EWJoint::getParent() {
    return parent;
}
void EWJoint::setAngle(float a) {
    clamp(a, m_minAngle, m_maxAngle);
    currentState.angle = a;
}
void EWJoint::setMaxAngle(float a) {
    m_maxAngle = a;
}
void EWJoint::setMinAngle(float a) {
    m_minAngle = a;
}
float EWJoint::getAngle() {
    return currentState.angle;
}
void EWJoint::reset() {
    currentState = defaultState;
}

void EWJoint::parentTransform()
{
    if(parent) {
        parent->parentTransform();
        glTranslatef(parent->getPosition().x,
                parent->getPosition().y, 0.0f);
        glRotatef(parent->getAngle(), 0.0f, 0.0f, 1.0f);
    }
}
void EWJoint::translateToMe() {
    parentTransform();
    glTranslatef(currentState.position.x,
                currentState.position.y, 0.0f);
}
void EWJoint::rotateToMe() {
    glRotatef(currentState.angle, 0.0f, 0.0f, 1.0f);
}

void EWJoint::update()
{
    clamp(currentState.angle, m_minAngle, m_maxAngle);
}

void EWJoint::draw()
{
    glPushMatrix();
        // Draw a connecting line between this joint and
        // its parent
        if(parent) {
            parentTransform();
            stroke(1.0f, 1.0f, 0.0f);
            line(vec2::Zero(), currentState.position);
        }
        glTranslatef(currentState.position.x, currentState.position.y, 0.0f);

        glPushMatrix();
            fill(0.0f, 0.0f, 1.0f);
            stroke(0.0f, 0.0f, 1.0f);
            glColor3f(0.0f, 0.0f, 1.0f);
            ellipse(vec2::Zero(), vec2(5.0f));
            noFill();
            stroke(0.0f, 1.0f, 0.0f);
            arc(vec2::Zero(), vec2(15.0f), m_minAngle, m_maxAngle);
        glPopMatrix();

        // Current angle
        rotateToMe();
        glPushMatrix();
            stroke(1.0f, 0.0f, 0.0f);
            strokeWeight(3);
            line(vec2::Zero(), vec2(25.0f, 0.0f));
            strokeWeight(1);
        glPopMatrix();
    glPopMatrix();
}








EggWalker::EggWalker()
{
    this->SetName("EggWalker");
    m_jMain = new EWJoint(EWJoint::State(this->m_position, 180.0f));

    m_jTorso = new EWJoint(EWJoint::State(vec2(-1.0f, 59.0f), 90.0f));
    m_jTorso->setParent(m_jMain);
    m_jTorso->setMinAngle(-30.0f);
    m_jTorso->setMaxAngle(90.0f);

    m_jHead = new EWJoint(EWJoint::State(vec2(89.0f, 14.0f), -90.0f));
    m_jHead->setParent(m_jTorso);
    m_jHead->setMinAngle(-180.0f);
    m_jHead->setMaxAngle(-90.0f);

    m_jBForearm = new EWJoint(EWJoint::State(vec2(73.0f, 9.0f), -180.0f));
    m_jBForearm->setParent(m_jTorso);
    m_jBForearm->setMinAngle(-240.0f);
    m_jBForearm->setMaxAngle(-120.0f);
    m_jBArm = new EWJoint(EWJoint::State(vec2(31.0f, -6.0f), 90.0f));
    m_jBArm->setParent(m_jBForearm);
    m_jBArm->setMinAngle(0.0f);
    m_jBArm->setMaxAngle(90.0f);

    m_jFForearm = new EWJoint(EWJoint::State(vec2(73.0f, 9.0f), -180.0f));
    m_jFForearm->setParent(m_jTorso);
    m_jFForearm->setMinAngle(-240.0f);
    m_jFForearm->setMaxAngle(-120.0f);
    m_jFArm = new EWJoint(EWJoint::State(vec2(31.0f, -6.0f), 90.0f));
    m_jFArm->setParent(m_jFForearm);
    m_jFArm->setMinAngle(0.0f);
    m_jFArm->setMaxAngle(90.0f);

    m_jBThigh = new EWJoint(EWJoint::State(vec2(-1.0f, 31.0f), 180.0f));
    m_jBThigh->setParent(m_jMain);
    m_jBThigh->setMinAngle(120.0f);
    m_jBThigh->setMaxAngle(240.0f);

    m_jFThigh = new EWJoint(EWJoint::State(vec2(-1.0f, 31.0f), 180.0f));
    m_jFThigh->setParent(m_jMain);
    m_jFThigh->setMinAngle(120.0f);
    m_jFThigh->setMaxAngle(240.0f);

    m_jBLeg = new EWJoint(EWJoint::State(vec2(30.0f, 0.0f), 90.0f));
    m_jBLeg->setParent(m_jBThigh);
    m_jBLeg->setMinAngle(90.0f);
    m_jBLeg->setMaxAngle(90.0f);

    m_jFLeg = new EWJoint(EWJoint::State(vec2(30.0f, 0.0f), 90.0f));
    m_jFLeg->setParent(m_jFThigh);
    m_jFLeg->setMinAngle(90.0f);
    m_jFLeg->setMaxAngle(90.0f);

    m_joints.reserve(9);
    m_joints.push_back(m_jMain);
    m_joints.push_back(m_jHead);
    m_joints.push_back(m_jBArm);
    m_joints.push_back(m_jBForearm);
    m_joints.push_back(m_jTorso);
    m_joints.push_back(m_jBThigh);
    m_joints.push_back(m_jFThigh);
    m_joints.push_back(m_jBLeg);
    m_joints.push_back(m_jFLeg);
    m_joints.push_back(m_jFForearm);
    m_joints.push_back(m_jFArm);
}

EggWalker::~EggWalker()
{
    m_joints.clear();
    delete m_jFArm;
    delete m_jFForearm;
    delete m_jBArm;
    delete m_jBForearm;
    delete m_jHead;
    delete m_jTorso;
    delete m_jBThigh;
    delete m_jFThigh;
    delete m_jBLeg;
    delete m_jFLeg;
    delete m_jMain;
}

void EggWalker::Initialize() {

}

void EggWalker::LoadContent() {
    m_tArm = ofTexturePool::LoadTexture("sprites/Bosses/EggWalker/tArm");
    m_tForearm = ofTexturePool::LoadTexture("sprites/Bosses/EggWalker/tForearm");
    m_tHead = ofTexturePool::LoadTexture("sprites/Bosses/EggWalker/tHead");
    m_tLeg = ofTexturePool::LoadTexture("sprites/Bosses/EggWalker/tLeg");
    // Rocket
    m_tShoulder = ofTexturePool::LoadTexture("sprites/Bosses/EggWalker/tpShoulder");
    m_tThigh = ofTexturePool::LoadTexture("sprites/Bosses/EggWalker/tpThigh");
    m_tTorso = ofTexturePool::LoadTexture("sprites/Bosses/EggWalker/tTorso");
}

void EggWalker::UnloadContent() {
    ofTexturePool::DisposeTexture(m_tArm);
    ofTexturePool::DisposeTexture(m_tForearm);
    ofTexturePool::DisposeTexture(m_tHead);
    ofTexturePool::DisposeTexture(m_tLeg);
    // Rocket
    ofTexturePool::DisposeTexture(m_tShoulder);
    ofTexturePool::DisposeTexture(m_tThigh);
    ofTexturePool::DisposeTexture(m_tTorso);
}

void EggWalker::Update() {
    for(auto joint : m_joints)
        joint->update();
    m_jMain->setPosition(m_position);
}

void EggWalker::drawPiece(ofTexture* t, EWJoint* j, vec2 hotspot)
{
    glPushMatrix();
    j->translateToMe();
    j->rotateToMe();
    t->Draw(vec2::Zero(), t->GetSize().toVec2(),
            vec2::Zero(), t->GetSize().toVec2(),
            0.0f, 1.0f, hotspot, Color4::MaskToColor4(WHITE));
    glPopMatrix();
}

void EggWalker::Draw() {

    // Back forearm
    //(necessary?)
    // Back arm
    drawPiece(m_tArm, m_jBArm, vec2(4.0f, 12.0f));
    // Back Thigh
    // TODO
    // Back Leg
    drawPiece(m_tLeg, m_jBLeg, vec2(45.0f, -5.0f));
    // Head
    drawPiece(m_tHead, m_jHead, vec2(18.0f, 3.0f));
    // Torso
    drawPiece(m_tTorso, m_jTorso, vec2(-14.0f, 47.0f));
    // Shoulder plat
    {
        glPushMatrix();
        m_jFForearm->translateToMe();
        glTranslatef(15.0f, -16.0f, 0.0f);
        glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
        m_tShoulder->Draw(vec2::Zero(), Color4::MaskToColor4(WHITE));
        glPopMatrix();
    }
    // Front Forearm
    // TODO
    // Front Arm
    drawPiece(m_tArm, m_jFArm, vec2(4.0f, 12.0f));
    // Front Thigh
    // TODO
    {
        glPushMatrix();
        vec2 thighPropPos = m_jFThigh->getPosition();
        thighPropPos += m_jFLeg->getPosition();
        thighPropPos /= 2.0f;
        m_jFThigh->translateToMe();
        glTranslatef(thighPropPos.x, thighPropPos.y, 0.0f);
        m_tThigh->Draw(vec2::Zero(), Color4::MaskToColor4(WHITE));
        glPopMatrix();
    }
    // Front Leg
    drawPiece(m_tLeg, m_jFLeg, vec2(45.0f, -5.0f));

    //for(auto joint : m_joints)
    //    joint->draw();
}