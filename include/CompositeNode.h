#include "Node.h"
#include <vector>
#include <memory>


class CompositeNode : public Node {
protected:
    std::vector<std::shared_ptr<Node>> children;
public:
    void addChild(std::shared_ptr<Node> child);
};
