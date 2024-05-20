#include "Player.h"
#include <iostream>
#include <cmath>

using namespace std;

Player::Player(){
    this->initTexture();
    this->initSprite();
    this->initAnimations();
    this->initPhysics();
}

Player::~Player(){
}

void Player::moveCollision(float x, float y) {
    this->sprite.move(x, y);
}

void Player::move(const float dir_x, const float dir_y, float deltaTime){
        this->velocity.x += dir_x * this->acceleration * deltaTime;
        this->velocity.y += dir_y * this->acceleration * deltaTime;
        if(abs(this->velocity.x) > this->maxVelocity) {
            this->velocity.x = this->maxVelocity * ((this->velocity.x < 0.f) ? -1.f : 1.f);
        }
}
//TODO: Can't jump while walking
void Player::updateMovement(float deltaTime)
{
    this->currentState = State::Normal;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)){ //Left
        this->move(-5.f, 0.f, deltaTime);
        this->currentState = State::Idle;
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)){ //Right
        this->move(5.f, 0.f, deltaTime);
        this->currentState = State::Walking;
    } /*else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)){ //Down (Crouch)
        this->move(0.f, 5.f);
        this->currentState = State::Crouching;
    }*/
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) { // Jump
        if (!this->keyPressed && this->canJump) {
            this->velocity.y = -sqrtf(2.0f * this->gravity * this->jumpHeight);
            this->currentState = State::Jumping;
            this->keyPressed = true;
            this->canJump = false;
        }
    } else {
        this->keyPressed = false; 
    }
}

void Player::updateAnimations(float deltaTime) {
    bool facingRight = this->sprite.getScale().x > 0; 

    int animationRow = 0; 
    switch(currentState) {
        case State::Walking: animationRow = 2; break; 
        case State::Jumping: animationRow = 5; break; 
        case State::Crouching: animationRow = 7; break;
        default: animationRow = 0; break;
    }


    animations[currentState].update(animationRow, deltaTime, facingRight); //TODO: Fix facingRight
    this->sprite.setTextureRect(animations[currentState].uvRect);
}

void Player::update(float deltaTime) {
    this->updateMovement(deltaTime);

    if(this->sprite.getPosition().y > 200.f) { //TODO: Modify to when the sprite is on the ground (collision from the botton)
        if (!this->keyPressed) { 
            this->canJump = true;
        }
    }

    if (currentState != State::Normal) {
        this->updateAnimations(deltaTime); 
    }

    this->updatePhysics();
}


void Player::render(sf::RenderTarget & target){
    target.draw(this->sprite);
}

void Player::updatePhysics() {
    //Gravity
    if(canJump == false) {
        this->velocity.y += 1.0 * this->gravity; //TODO: apply only when falling (modify with onGround)
    }
    if(abs(this->velocity.x) > this->maxVelocityY) {
        this->velocity.y = this->maxVelocityY * ((this->velocity.y < 0.f) ? -1.f : 1.f);
    }

    this->velocity *= this->deceleration;
    if(abs(this->velocity.x) < this->minVelocity)
        this->velocity.x = 0.f;
    if(abs(this->velocity.y) < this->minVelocity)
        this->velocity.y = 0.f;
    this->sprite.move(this->velocity);
}

void Player::initTexture(){
    if(!this->textureSheet.loadFromFile("resources/sprites/knight.png")){
        cout << "Error loading player sprite sheet" << endl;
    }

}

bool Player::isColliding(const sf::FloatRect &other) const {
    float adjustmentFactor = 10.f; //Fais pas gaffe c'est une solution pansement en attendant

    sf::FloatRect globalBounds = this->sprite.getGlobalBounds();

    sf::FloatRect adjustedGlobalBounds(
        globalBounds.left + adjustmentFactor,
        globalBounds.top + adjustmentFactor,
        globalBounds.width - 2 * adjustmentFactor,
        globalBounds.height - 2 * adjustmentFactor
    );

    return adjustedGlobalBounds.intersects(other); //TODO: Utiliser le getHitbox a la place
}


void Player::resolveCollision(const sf::FloatRect &other) {
    sf::FloatRect playerBounds = this->sprite.getGlobalBounds();
    
    float overlapLeft = (playerBounds.left + playerBounds.width) - other.left;
    float overlapRight = (other.left + other.width) - playerBounds.left;
    float overlapTop = (playerBounds.top + playerBounds.height) - other.top;
    float overlapBottom = (other.top + other.height) - playerBounds.top;

    bool fromLeft = fabs(overlapLeft) < fabs(overlapRight);
    bool fromTop = fabs(overlapTop) < fabs(overlapBottom);

    float minOverlapX = fromLeft ? overlapLeft : overlapRight;
    float minOverlapY = fromTop ? overlapTop : overlapBottom;

    float adjustmentFactor = 10.f;

    if (fabs(minOverlapX) < fabs(minOverlapY)) {
        if (fromLeft) {
            this->moveCollision(-minOverlapX + adjustmentFactor, 0);
        } else {
            this->moveCollision(minOverlapX - adjustmentFactor, 0);
        }
    } else {
        if (fromTop) {
            this->moveCollision(0, -minOverlapY + adjustmentFactor);
        } else {
            this->moveCollision(0, minOverlapY - adjustmentFactor);
        }
    }
}

//TODO: Collisions with windows borders
void Player::initSprite(){
    this->sprite.setTexture(this->textureSheet);
    this->sprite.setTextureRect(sf::IntRect(0,0,32,32));

    this->sprite.setScale(3.0f,3.0f);
    this->sprite.setPosition(10.f, 210.f);
}

void Player::initAnimations() {
    animations[Idle] = Animation(&textureSheet, sf::Vector2u(4, 1), 0.3f, sf::Vector2u(32,32));       
    animations[Walking] = Animation(&textureSheet, sf::Vector2u(8, 2), 0.09f, sf::Vector2u(32,32));    
    animations[Jumping] = Animation(&textureSheet, sf::Vector2u(5, 1), 0.15f, sf::Vector2u(32,32));   
    animations[Crouching] = Animation(&textureSheet, sf::Vector2u(3, 1), 0.2f, sf::Vector2u(32,32));  
}

void Player::initPhysics() {
    this->maxVelocity = 7.f;
    this->minVelocity = 1.f;
    this->acceleration = 120.f;
    this->deceleration = 0.9f;
    this->gravity = 2.f;
    this->maxVelocityY = 15.f;
    this->jumpHeight = 100.f;
    this->jumpVelocity = 0.0f;
    /*
    this->airControl = 200.0f; 
    this->airBrake = 0.5f;
    */ 
}
