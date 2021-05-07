#include <utility>

#include "Context.h"
#include "mock_ptr.h"
#include "SolitaireMock.h"
#include "cards/Card.h"
#include "colliders/FoundationPileColliderMock.h"
#include "colliders/TableauPileColliderMock.h"
#include "gtest/gtest.h"

using namespace testing;
using namespace solitaire::colliders;
using namespace solitaire::geometry;
using namespace solitaire::piles;

namespace solitaire {

namespace {
constexpr unsigned foundationPilesCount {4};
constexpr unsigned tableauPilesCount {7};
}

class ContextTests: public Test {
public:
    using FoundationPileColliderMocks =
        std::array<mock_ptr<FoundationPileColliderMock>, foundationPilesCount>;

    using TableauPileColliderMocks =
        std::array<mock_ptr<TableauPileColliderMock>, tableauPilesCount>;

    ContextTests() {
        Context::FoundationPileColliders foundationPileColliders;
        std::transform(
            foundationPileColliderMocks.begin(), foundationPileColliderMocks.end(),
            foundationPileColliders.begin(), [](auto& collider) {
                return collider.make_unique();
            }
        );

        Context::TableauPileColliders tableauPileColliders;
        std::transform(
            tableauPileColliderMocks.begin(), tableauPileColliderMocks.end(),
            tableauPileColliders.begin(), [](auto& collider) {
                return collider.make_unique();
            }
        );

        context = std::make_unique<Context>(
            solitaireMock.make_unique(), std::move(foundationPileColliders),
            std::move(tableauPileColliders));
    }

    mock_ptr<SolitaireMock> solitaireMock;
    FoundationPileColliderMocks foundationPileColliderMocks;
    TableauPileColliderMocks tableauPileColliderMocks;
    std::unique_ptr<Context> context;
};

TEST_F(ContextTests, getSolitaire) {
    EXPECT_EQ(&context->getSolitaire(), solitaireMock.get());
    EXPECT_EQ(&std::as_const(context)->getSolitaire(), solitaireMock.get());
}

TEST_F(ContextTests, getFoundationPileColliders) {
    for (PileId id {0}; id < foundationPilesCount; ++id) {
        EXPECT_EQ(&context->getFoundationPileCollider(id),
                  foundationPileColliderMocks[id].get());
        EXPECT_EQ(&std::as_const(*context).getFoundationPileCollider(id),
                  foundationPileColliderMocks[id].get());
    }
}

TEST_F(ContextTests, throwOnInvalidFoundationPileColliderId) {
    EXPECT_THROW(
        context->getFoundationPileCollider(PileId {foundationPilesCount}),
        std::runtime_error
    );

    EXPECT_THROW(
        std::as_const(*context).getFoundationPileCollider(PileId {foundationPilesCount}),
        std::runtime_error
    );
}

TEST_F(ContextTests, getTableauPileColliders) {
    for (PileId id {0}; id < tableauPilesCount; ++id) {
        EXPECT_EQ(&context->getTableauPileCollider(id),
                  tableauPileColliderMocks[id].get());
        EXPECT_EQ(&std::as_const(*context).getTableauPileCollider(id),
                  tableauPileColliderMocks[id].get());
    }
}

TEST_F(ContextTests, throwOnInvalidTableauPileColliderId) {
    EXPECT_THROW(
        context->getTableauPileCollider(PileId {tableauPilesCount}),
        std::runtime_error
    );

    EXPECT_THROW(
        std::as_const(*context).getTableauPileCollider(PileId {tableauPilesCount}),
        std::runtime_error
    );
}

TEST_F(ContextTests, setAndGetPositions) {
    Position position1 {1, 7};
    Position position2 {2, 4};

    context->setMousePosition(position1);
    EXPECT_EQ(context->getMousePosition(), position1);
    context->setMousePosition(position2);
    EXPECT_EQ(context->getMousePosition(), position2);
    context->setCardsInHandPosition(position1);
    EXPECT_EQ(context->getCardsInHandPosition(), position1);
    context->setCardsInHandPosition(position2);
    EXPECT_EQ(context->getCardsInHandPosition(), position2);
}

}
