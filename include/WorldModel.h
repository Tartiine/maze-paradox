#ifndef WORLD_MODEL_H
#define WORLD_MODEL_H

#include <string>
#include <variant>
#include <unordered_map>
#include "SFML/Graphics.hpp"
#include "Action.h"
#include "Goal.h"
#include "WorldDiscover.h"

using namespace std;

using WorldValue = variant<int, float, bool, string, sf::Vector2f>;

class WorldModel {
    public:
        WorldModel();
        WorldModel(const WorldModel& model);
        ~WorldModel() = default;

        void setState(const string& key, const WorldValue& value);
        template <typename T>
        T getState(const string& key) const;
        void addState(const string& key, const WorldValue& value);

        bool has(const string& key) const;

        void addAction(const Action& action);
        void addGoal(const Goal& goal);

        bool isDiscovered() const;
        void resetDiscovery();

        vector<Action> getAllPossibleActions() const;
        float getWorldDiscontentment() const;
        
    private:
        unordered_map<string, WorldValue> state;
        vector<Action> actions;
        vector<Goal> goals;
        bool isDiscovered;

};

#endif // WORLD_MODEL_H