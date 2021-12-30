#include <array>
#include <cassert>
#include <string>
#include <queue>
#include <unordered_map>
#include <map>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <set>

using std::array;
using std::swap;
using std::string;
using std::queue;
using std::unordered_map;
using std::ostream;

const int SideSize = 4;
const int FieldSize = SideSize * SideSize;
const array<char, FieldSize> FinishField( { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0 } );

std::map<char, std::pair<char, char>> correct_position = {
        {1, {0, 0}},
        {2, {0, 1}},
        {3, {0, 2}},
        {4, {0, 3}},
        {5, {1, 0}},
        {6, {1, 1}},
        {7, {1, 2}},
        {8, {1, 3}},
        {9, {2, 0}},
        {10, {2, 1}},
        {11, {2, 2}},
        {12, {2, 3}},
        {13, {3, 0}},
        {14, {3, 1}},
        {15, {3, 2}},
        {0, {3, 3}},
};

/*std::pair<char, char> GetPosition(int number) {
    if (number != 0) {
        return {(number - 1) / 4,(number - 1) % 4};
    } else {
        return {3,3};
    }
}*/

int GetLinearConflicts(const std::array<char, 16>& state) {
    int result = 0;
    // проверяем строки
    for (char i = 0; i < 4; ++i) {
        bool is_conflict = false;
        for (char j = 0; j < 4; ++j) {

            if (correct_position[state[i * 4 + j]].first == i &&
                correct_position[state[i * 4 + j]].second != j &&
                state[i * 4 + j] != 0) {
                if (correct_position[state[i * 4 + j]].second > j) {
                    for (char k = j + 1; k < 4; ++k) {
                        if (correct_position[state[i * 4 + k]].first == i &&
                            correct_position[state[i * 4 + k]].second <
                            correct_position[state[i * 4 + j]].second &&
                            state[i * 4 + k] != 0) {
//                            std::cout << int(state[i * 4 + j]) << " -> " << int(state[i * 4 + k]) << std::endl;
                            ++result;
                            is_conflict = true;
                        }
                    }

                } else {
                    for (char k = 0; k < j; ++k) {
                        if (correct_position[state[i * 4 + k]].first == i &&
                            correct_position[state[i * 4 + k]].second >
                            correct_position[state[i * 4 + j]].second &&
                            state[i * 4 + k] != 0) {
//                            std::cout << int(state[i * 4 + j]) << " <- " << int(state[i * 4 + k]) << std::endl;
                            ++result;
                        }
                    }
                }
                //std::cout << '|' << int(state[i * 4 + j]);
            }
        }
        if(is_conflict) {
          //  --result;
        }
    }

    // проверяем столбцы
    for (char j = 0; j < 4; ++j) {
        bool is_conflict = false;
        for (char i = 0; i < 4; ++i) {
            if (correct_position[state[i * 4 + j]].second == j &&
                correct_position[state[i * 4 + j]].first != i &&
                state[i * 4 + j] != 0) {
                if (correct_position[state[i * 4 + j]].first > i) {
                    for (char k = i + 1; k < 4; ++k) {
                        if (correct_position[state[k * 4 + j]].second == j &&
                            correct_position[state[k * 4 + j]].first <
                            correct_position[state[i * 4 + j]].first &&
                            state[k * 4 + j] != 0) {
//                            std::cout << int(state[i * 4 + j]) << " \\/ " << int(state[k * 4 + j]) << std::endl;
                            ++result;
                            is_conflict = true;
                        }
                    }
                } else {
                    for (char k = 0; k < i; ++k) {
                        if (correct_position[state[k * 4 + j]].second == j &&
                            correct_position[state[k * 4 + j]].first >
                            correct_position[state[i * 4 + j]].first &&
                            state[k * 4 + j] != 0) {
//                            std::cout << int(state[i * 4 + j]) << " /\\ " << int(state[k * 4 + j]) << std::endl;
                            ++result;
                        }
                    }
                }
                // std::cout << '-' << int(state[i * 4 + j]);
            }
        }
        if(is_conflict) {
          //  --result;
        }
    }
//    std::cout << result << std::endl;
    result/=2;

    return result;
}



int GetMetric(const std::array<char, 16>& state) {
    int metric = 0;
    for (char i = 0; i < 15; ++i) {
        metric += std::abs(correct_position[i + 1].first - correct_position[state[i]].first) +
                  std::abs(correct_position[i + 1].second - correct_position[state[i]].second);
    }
    metric += std::abs(correct_position[0].first - correct_position[state[15]].first) +
              std::abs(correct_position[0].second - correct_position[state[15]].second);

    metric /= 2;

    metric +=GetLinearConflicts(state);
    return metric;
}



class GameState {
public:
    GameState( const array<char, FieldSize>& _field );

    bool CanMoveLeft() const;
    bool CanMoveUp() const;
    bool CanMoveRight() const;
    bool CanMoveDown() const;

    GameState MoveLeft() const;
    GameState MoveUp() const;
    GameState MoveRight() const;
    GameState MoveDown() const;

    bool IsFinish() const;

    bool operator == ( const GameState& state ) const { return  field == state.field; }
    bool operator != ( const GameState& state ) const { return  !(*this == state); }

    array<char, FieldSize>& GetField() { return field;}

    friend ostream& operator << ( ostream& out, const GameState& state );
    friend struct Hasher;

private:
    array<char, FieldSize> field;
    char zeroPos;
};

GameState::GameState( const array<char, FieldSize>& _field ) :
        field( _field ),
        zeroPos( -1 )
{
    for( size_t i = 0; i < field.size(); ++i ) {
        if( field[i] == 0 ) {
            zeroPos = i;
            break;
        }
    }
    assert( zeroPos != -1 );
}

bool GameState::CanMoveLeft() const
{
    return zeroPos % SideSize != 0;
}

bool GameState::CanMoveUp() const
{
    return zeroPos >= SideSize;
}

bool GameState::CanMoveRight() const
{
    return zeroPos % SideSize < SideSize - 1;
}

bool GameState::CanMoveDown() const
{
    return zeroPos < FieldSize - SideSize;
}

GameState GameState::MoveLeft() const
{
    assert( CanMoveLeft() );
    GameState newState( *this );
    swap( newState.field[newState.zeroPos], newState.field[newState.zeroPos - 1] );
    --newState.zeroPos;
    return newState;
}

GameState GameState::MoveUp() const
{
    assert( CanMoveUp() );
    GameState newState( *this );
    swap( newState.field[newState.zeroPos], newState.field[newState.zeroPos - SideSize] );
    newState.zeroPos -= SideSize;
    return newState;
}

GameState GameState::MoveRight() const
{
    assert( CanMoveRight() );
    GameState newState( *this );
    swap( newState.field[newState.zeroPos], newState.field[newState.zeroPos + 1] );
    ++newState.zeroPos;
    return newState;
}

GameState GameState::MoveDown() const
{
    assert( CanMoveDown() );
    GameState newState( *this );
    swap( newState.field[newState.zeroPos], newState.field[newState.zeroPos + SideSize] );
    newState.zeroPos += SideSize;
    return newState;
}

bool GameState::IsFinish() const
{
    return field == FinishField;
}

ostream& operator << ( ostream& out, const GameState& state )
{
    for( int y = 0; y < SideSize; ++y ) {
        for( int x = 0; x < SideSize; ++x ) {
            out << static_cast<int>(state.field[y * SideSize + x]) << ' ';
        }
        out << std::endl;
    }
    out << std::endl;
    return out;
}

struct Hasher {
    size_t operator() ( const GameState& state ) const {
        size_t hash = 0;
        memcpy( &hash, &state.field[0], sizeof( hash ) );
        return hash;
    }
};

//string Get8thSolution( const GameState& state )
//{
//    queue<GameState> bfsQueue;
//    bfsQueue.push( state );
//    unordered_map<GameState, char, Hasher> visited;
//    visited[state] = 'S';
//    while( bfsQueue.size() > 0 ) {
//        GameState tempState = bfsQueue.front();
//        bfsQueue.pop();
//        if( tempState.IsFinish() ) {
//            break;
//        }
//        if( tempState.CanMoveLeft() ) {
//            GameState newState = tempState.MoveLeft();
//            if( visited.find( newState ) == visited.end() ) {
//                visited[newState] = 'L';
//                bfsQueue.push( newState );
//            }
//        }
//        if( tempState.CanMoveUp() ) {
//            GameState newState = tempState.MoveUp();
//            if( visited.find( newState ) == visited.end() ) {
//                visited[newState] = 'U';
//                bfsQueue.push( newState );
//            }
//        }
//        if( tempState.CanMoveRight() ) {
//            GameState newState = tempState.MoveRight();
//            if( visited.find( newState ) == visited.end() ) {
//                visited[newState] = 'R';
//                bfsQueue.push( newState );
//            }
//        }
//        if( tempState.CanMoveDown() ) {
//            GameState newState = tempState.MoveDown();
//            if( visited.find( newState ) == visited.end() ) {
//                visited[newState] = 'D';
//                bfsQueue.push( newState );
//            }
//        }
//    }
//
//    string result;
//    GameState tempState( FinishField );
//    char move = visited[tempState];
//    while( move != 'S' ) {
//        result += move;
//        switch( move ) {
//            case 'L':
//                tempState = tempState.MoveRight();
//                break;
//            case 'U':
//                tempState = tempState.MoveDown();
//                break;
//            case 'R':
//                tempState = tempState.MoveLeft();
//                break;
//            case 'D':
//                tempState = tempState.MoveUp();
//                break;
//            default:
//                assert( false );
//        }
//        move = visited[tempState];
//    }
//    std::reverse( result.begin(), result.end() );
//    return result;
//}
string Get8thSolution( const GameState& state )
{
    struct StateWithWeight{
        GameState state;
        int weight;
        StateWithWeight(const GameState& state, int weight) : state(state), weight(weight) {}
        bool operator < (const StateWithWeight& other_state) const {return (this->weight < other_state.weight);}
        bool operator == (const StateWithWeight& other_state) const {return (this->weight == other_state.weight);}
        bool operator > (const StateWithWeight& other_state) const {return (this->weight > other_state.weight);}
    };

    queue<GameState> bfsQueue;
    bfsQueue.push( state );
    unordered_map<GameState, char, Hasher> visited;
    visited[state] = 'S';
    while( bfsQueue.size() > 0 ) {
        GameState tempState = bfsQueue.front();
        bfsQueue.pop();
        if( tempState.IsFinish() ) {
            break;
        }

        std::multiset<StateWithWeight> current_variances;

        if( tempState.CanMoveLeft() ) {
            GameState newState = tempState.MoveLeft();
            if( visited.find( newState ) == visited.end() ) {
                visited[newState] = 'L';
                //bfsQueue.push( newState );
                current_variances.emplace(newState, GetMetric(newState.GetField()));
//                current_variances.insert(field_pair(GetMetric(newState.GetField()), newState));
            }
        }
        if( tempState.CanMoveUp() ) {
            GameState newState = tempState.MoveUp();
            if( visited.find( newState ) == visited.end() ) {
                visited[newState] = 'U';
//                bfsQueue.push( newState );
                current_variances.emplace(newState, GetMetric(newState.GetField()));
//                current_variances.insert(field_pair(GetMetric(newState.GetField()), newState));
            }
        }
        if( tempState.CanMoveRight() ) {
            GameState newState = tempState.MoveRight();
            if( visited.find( newState ) == visited.end() ) {
                visited[newState] = 'R';
//                bfsQueue.push( newState );
                current_variances.emplace(newState, GetMetric(newState.GetField()));
//                current_variances.insert(field_pair(GetMetric(newState.GetField()), newState));
            }
        }
        if( tempState.CanMoveDown() ) {
            GameState newState = tempState.MoveDown();
            if( visited.find( newState ) == visited.end() ) {
                visited[newState] = 'D';
//                bfsQueue.push( newState );
                current_variances.emplace(newState, GetMetric(newState.GetField()));
//                current_variances.insert(field_pair(GetMetric(newState.GetField()), newState));
            }
        }

        for (auto& field: current_variances) {
            bfsQueue.push(field.state);
        }
    }

    string result;
    GameState tempState( FinishField );
    char move = visited[tempState];
    while( move != 'S' ) {
        result += move;
        switch( move ) {
            case 'L':
                tempState = tempState.MoveRight();
                break;
            case 'U':
                tempState = tempState.MoveDown();
                break;
            case 'R':
                tempState = tempState.MoveLeft();
                break;
            case 'D':
                tempState = tempState.MoveUp();
                break;
            default:
                assert( false );
        }
        move = visited[tempState];
    }
    //std::reverse( result.begin(), result.end() );
    return result;
}


bool IsCorrect(const array<char, FieldSize>& StartField) {
    int inversions_numb = 0;
    char null_row = 0;

    for(int i = 0; i < 15; ++i) {
        if (StartField[i] != 0) {
            for(int j = i + 1; j < 16; ++j) {
                if (StartField[j] != 0) {
                    if (StartField[i] > StartField[j]) {
                        ++inversions_numb;
                    }
                }
            }
        } else {
            null_row = i / 4;
        }
    }

    return !((inversions_numb + null_row) % 2);
}

int main()
{
    const array<char, FieldSize> startField( { 1, 2, 3, 4, 5, 6, 7, 8, 9, 14, 11, 12, 13, 10, 0, 15 } );
    GameState state( startField );
    if (IsCorrect(startField)) {
        //std::cout << state;
        string solution = Get8thSolution( state );
        std::cout << solution.size() << std::endl;
        for( char move : solution ) {
            switch (move) {
                case 'L':
                    state = state.MoveLeft();
                    std::cout << 'R' << std::endl;
                    break;
                case 'U':
                    // state = state.MoveUp();
                    std::cout << 'D' << std::endl;
                    break;
                case 'R':
                    //state = state.MoveRight();
                    std::cout << 'L' << std::endl;
                    break;
                case 'D':
                    //state = state.MoveDown();
                    std::cout << 'U' << std::endl;
                    break;
                default:
                    assert(false);
            }
        }
            //std::cout << state;
    } else {
        std::cout << -1;
    }


    return 0;
}


