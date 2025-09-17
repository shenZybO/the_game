#include "collision_system.h"
#include <algorithm>

CollisionSystem& CollisionSystem::Instance() {
    static CollisionSystem inst;
    return inst;
}

void CollisionSystem::RegisterListener(ICollisionListener* listener) {
    if (!listener) return;
    for (auto* l : listeners)
        if (l == listener) return;
    listeners.push_back(listener);
}

void CollisionSystem::UnregisterListener(ICollisionListener* listener) {
    listeners.erase(std::remove(listeners.begin(), listeners.end(), listener), listeners.end());
}

void CollisionSystem::Update(const std::vector<std::unique_ptr<Actor>>& actors, Actor* player) {
    // Gather all actors into a single list for collision checks
    std::vector<Actor*> all;
    all.reserve(actors.size() + (player ? 1 : 0));
    for (auto& a : actors)
        if (a) all.push_back(a.get());
    if (player) all.push_back(player);

    for (ICollisionListener* listener : listeners) {
        if (!listener) continue;
        Actor& selfActor = listener->GetCollisionActor();
        // skip if self actor is not alive
        if (!selfActor.IsAlive()) continue;

        // check collision against all other actors
        Rectangle selfRect = selfActor.GetRect();
        for (Actor* other : all) {
            if (other == &selfActor) continue;
            if (!other->IsAlive()) continue;
            Rectangle otherRect = other->GetRect();
            if (CheckCollisionRecs(selfRect, otherRect)) {
                // compute overlap rectangle and notify listener
                float left = std::max(selfRect.x, otherRect.x);
                float top = std::max(selfRect.y, otherRect.y);
                float right = std::min(selfRect.x + selfRect.width, otherRect.x + otherRect.width);
                float bottom = std::min(selfRect.y + selfRect.height, otherRect.y + otherRect.height);
                Rectangle overlap{left, top, right - left, bottom - top};
                listener->OnCollision(selfActor, *other, overlap);
            }
        }
    }
}
