//
// Created by arthur wesley on 8/7/21.
//

#include <map>
#include <ciso646>

#include "Numpy.h"

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
            {"Ko Points", KO_POINTS},
            {"black_stones", BLACK_STONES},
            {"white_stones", WHITE_STONES},
            {"empty_points", EMPTY_POINTS},
            {"ko_points", KO_POINTS}
        };

        void getNextBlackStone(const GoGame& game, int8_t* buffer_ptr, unsigned bufferIndex, Vertex toCheck);
        void getNextWhiteStone(const GoGame& game, int8_t* buffer_ptr, unsigned bufferIndex, Vertex VertextoCheck);
        void getNextEmptySpace(const GoGame& game, int8_t* buffer_ptr, unsigned bufferIndex, Vertex toCheck);
        void getNextKoPoint(const GoGame& game, int8_t* buffer_ptr, unsigned bufferIndex, Vertex toCheck);

        /**
         *
         * Generate a features matrix for a given go game
         *
         * @param game the game to generate the features vector for
         * @param features list of features to Include in the game
         * @return a numpy array containing desired features
         */
        py::array_t<uint8_t> getFeatures(const GoGame& game, const std::vector<feature>& features){

            unsigned side = game.getSide();

            auto result = py::array_t<int8_t>(side * side * features.size());
            auto buffer = result.request(true);

            auto* buffer_ptr = (int8_t*) buffer.ptr;

            for (unsigned i = 0; i < side; i++){
                for (unsigned j = 0; j < side; j++){

                    unsigned featureOffset = 0;
                    unsigned boardOffset = (side * i + j) * features.size();

                    for (const auto& item : features){
                        switch (item){
                            case BLACK_STONES:
                                getNextBlackStone(game, buffer_ptr, boardOffset + featureOffset, {i, j});
                                break;
                            case WHITE_STONES:
                                getNextWhiteStone(game, buffer_ptr, boardOffset + featureOffset, {i, j});
                                break;
                            case EMPTY_POINTS:
                                getNextEmptySpace(game, buffer_ptr, boardOffset + featureOffset, {i, j});
                                break;
                            case KO_POINTS:
                                getNextKoPoint(game, buffer_ptr, boardOffset + featureOffset, {i, j});
                                break;
                        }
                        featureOffset++;
                    }

                }
            }

            result.resize({side, side, unsigned(features.size())});

            return result;

        }

        /**
         *
         * insert the black stones into a numpy array
         *
         * @param game game to get the stones from
         * @param buffer_ptr the pointer to the insertion buffer
         * @param bufferIndex the index to insert the next point at
         * @param toCheck the point to check
         */
        void getNextBlackStone(const GoGame& game, int8_t* buffer_ptr, unsigned bufferIndex, Vertex toCheck){

            // get the stone from the board
            auto stone = game.getSpace(toCheck);

            if (stone == BLACK){
                buffer_ptr[bufferIndex] = 1;
            }
            else {
                buffer_ptr[bufferIndex] = 0;
            }

        }

        /**
         *
         * insert the white stones into a numpy array
         *
         * @param game game to get the stones from
         * @param buffer_ptr the pointer to the insertion buffer
         * @param bufferIndex the index to insert the next point at
         * @param toCheck the point to check
         */
        void getNextWhiteStone(const GoGame& game, int8_t* buffer_ptr, unsigned bufferIndex, Vertex toCheck){

            // get the stone from the board
            auto stone = game.getSpace(toCheck);

            if (stone == WHITE){
                buffer_ptr[bufferIndex] = 1;
            }
            else {
                buffer_ptr[bufferIndex] = 0;
            }
        }

        /**
        *
        * insert the empty spaces into a numpy array
        *
        * @param game game to get the stones from
        * @param buffer_ptr the pointer to the insertion buffer
        * @param bufferIndex the index to insert the next point at
        * @param toCheck the point to check
        */
        void getNextEmptySpace(const GoGame& game, int8_t* buffer_ptr, unsigned bufferIndex, Vertex toCheck){

            // get the stone from the board
            auto stone = game.getSpace(toCheck);

            if (stone == EMPTY){
                buffer_ptr[bufferIndex] = 1;
            }
            else {
                buffer_ptr[bufferIndex] = 0;
            }

        }


        /**
         *
         * insert the ko points into a numpy array
         *
         * @param game game to get the stones from
         * @param buffer_ptr the pointer to the insertion buffer
         * @param bufferIndex the index to insert the next point at
         * @param toCheck the point to check
         */
        void getNextKoPoint(const GoGame& game, int8_t* buffer_ptr, unsigned bufferIndex, Vertex toCheck){

            Vertex ko = game.getKoPoint();

            if (ko.first == toCheck.first and ko.second == toCheck.second){
                buffer_ptr[bufferIndex] = 1;
            }
            else {
                buffer_ptr[bufferIndex] = 0;
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
