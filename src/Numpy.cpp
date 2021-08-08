//
// Created by arthur wesley on 8/7/21.
//

#include <map>
#include <ciso646>

#include "include/Numpy.h"

namespace sente {
    namespace utils {

        enum feature {
            BLACK_STONES,
            WHITE_STONES,
            EMPTY_POINTS,
            KO_POINTS
        };

        std::map<std::string, feature> featureMap {
            {"Black Stones", BLACK_STONES},
            {"White Stones", WHITE_STONES},
            {"Empty Points", EMPTY_POINTS},
            {"Ko Points", KO_POINTS}
        };

        void getBlackStones(const GoGame& game, int8_t* buffer_ptr, unsigned& currentIndex);
        void getWhiteStones(const GoGame& game, int8_t* buffer_ptr, unsigned& currentIndex);
        void getEmptySpaces(const GoGame& game, int8_t* buffer_ptr, unsigned& currentIndex);
        void getKoPoints(const GoGame& game, int8_t* buffer_ptr, unsigned& currentIndex);

        /**
         *
         * Generate a features matrix for a given go game
         *
         * @param game the game to generate the features vector for
         * @param features list of features to include in the game
         * @return a numpy array containing desired features
         */
        py::array_t<uint8_t> getFeatures(const GoGame& game, const std::vector<feature>& features){

            unsigned side = game.getBoard().getSide();

            auto result = py::array_t<int8_t>(side * side * features.size());
            auto buffer = result.request(true);

            auto* buffer_ptr = (int8_t*) buffer.ptr;

            unsigned index = 0;

            for (const auto& item : features){
                switch (item){
                    case BLACK_STONES:
                        getBlackStones(game, buffer_ptr, index);
                        break;
                    case WHITE_STONES:
                        getWhiteStones(game, buffer_ptr, index);
                        break;
                    case EMPTY_POINTS:
                        getEmptySpaces(game, buffer_ptr, index);
                        break;
                    case KO_POINTS:
                        getKoPoints(game, buffer_ptr, index);
                        break;
                }
            }

            result.resize({unsigned(features.size()), side, side});

            return result;

        }

        /**
         *
         * insert the black stones into a numpy array
         *
         * @param game game to get the stones from
         * @param buffer_ptr the pointer to the insertion buffer
         * @param currentIndex current index in the insertion buffer
         */
        void getBlackStones(const GoGame& game, int8_t* buffer_ptr, unsigned& currentIndex){

            unsigned side = game.getBoard().getSide();

            for (unsigned i = 0; i < side; i++){
                for (unsigned j = 0; j < side; j++){

                    // get the stone from the board
                    auto stone = game.getBoard().getStone({i, j});

                    if (stone == BLACK){
                        buffer_ptr[currentIndex++] = 1;
                    }
                    else {
                        buffer_ptr[currentIndex++] = 0;
                    }

                }
            }
        }

        /**
         *
         * insert the white stones into a numpy array
         *
         * @param game game to get the stones from
         * @param buffer_ptr the pointer to the insertion buffer
         * @param currentIndex current index in the insertion buffer
         */
        void getWhiteStones(const GoGame& game, int8_t* buffer_ptr, unsigned& currentIndex){

            unsigned side = game.getBoard().getSide();

            for (unsigned i = 0; i < side; i++){
                for (unsigned j = 0; j < side; j++){

                    // get the stone from the board
                    auto stone = game.getBoard().getStone({i, j});

                    if (stone == WHITE){
                        buffer_ptr[currentIndex++] = 1;
                    }
                    else {
                        buffer_ptr[currentIndex++] = 0;
                    }

                }
            }
        }

        /**
        *
        * insert the empty spaces into a numpy array
        *
        * @param game game to get the stones from
        * @param buffer_ptr the pointer to the insertion buffer
        * @param currentIndex current index in the insertion buffer
        */
        void getEmptySpaces(const GoGame& game, int8_t* buffer_ptr, unsigned& currentIndex){

            unsigned side = game.getBoard().getSide();

            for (unsigned i = 0; i < side; i++){
                for (unsigned j = 0; j < side; j++){

                    // get the stone from the board
                    auto stone = game.getBoard().getStone({i, j});

                    if (stone == EMPTY){
                        buffer_ptr[currentIndex++] = 1;
                    }
                    else {
                        buffer_ptr[currentIndex++] = 0;
                    }

                }
            }
        }


        /**
         *
         * insert the ko points into a numpy array
         *
         * @param game game to get the stones from
         * @param buffer_ptr the pointer to the insertion buffer
         * @param currentIndex current index in the insertion buffer
         */
        void getKoPoints(const GoGame& game, int8_t* buffer_ptr, unsigned& currentIndex){

            Point ko = game.getKoPoint();
            unsigned side = game.getBoard().getSide();

            for (unsigned i = 0; i < side; i++){
                for (unsigned j = 0; j < side; j++){
                    if (ko.first == i and ko.second == j){
                        buffer_ptr[currentIndex++] = 1;
                    }
                    else {
                        buffer_ptr[currentIndex++] = 0;
                    }
                }
            }

        }

        py::array_t<uint8_t> getFeatures(const GoGame& game, const std::vector<std::string>& features) {

            auto featureVector = std::vector<feature>();

            for (const auto& item : features){
                featureVector.push_back(featureMap[item]);
            }

            return getFeatures(game, featureVector);

        }

    }
}
