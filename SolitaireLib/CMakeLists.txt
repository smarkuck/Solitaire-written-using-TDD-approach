add_library(SolitaireLib STATIC)
target_include_directories(SolitaireLib PUBLIC headers)

target_sources(SolitaireLib PRIVATE
    sources/cards/Card.cpp
    sources/cards/ShuffledCardsGenerator.cpp
    sources/piles/DefaultFoundationPile.cpp
    sources/piles/DefaultStockPile.cpp
    sources/piles/DefaultTableauPile.cpp
)

add_subdirectory(unitTests)
