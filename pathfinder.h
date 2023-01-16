#ifndef pathfinder
#define pathfinder
#include "pathfinder_prototype.h"

/*
* @use convert board index int to board coordinate (int,int)
*/
std::vector<int> indexToCoor (const int index) {
    int row = index/BOARD_COL;
    int col = index%BOARD_COL; 
    return {row,col}; 
}

float minDis (const int i , const int j , const float diagToHorizontalWeight = DIAGONAL_WEIGHT) {
    if (i==j) return 0; 
    std::vector<int> first = indexToCoor(i);
    std::vector<int> second = indexToCoor(j);
    int diffX = abs(first[0] - second[0]); 
    int diffY = abs(first[1] - second[1]); 
    //same row or col 
    if (diffX == 0 || diffY == 0) return  diffX+diffY; 
    /*
    * not same row or col  
    * std::max(diffX,diffY) - std::min(diffX,diffY) is the number of non diagonal edges walked
    * std::min(diffX,diffY) is the number of diagonal edges walked
    */
    return (std::max(diffX,diffY) - std::min(diffX,diffY)) + std::min(diffX,diffY) * diagToHorizontalWeight; 
}

void preProcessor (const std::vector<int> in , const std::vector<int> out , const int lead , TreeNode& root , std::vector<finRuneTypeNode>& FRTN) {
    root.board = in;
    root.leadv.push_back(lead); 
    std::vector<int> runeTypeV; //contains the name of all rune types on in board 
    //initialize runeTypeV 
    for (int i = 0; i < in.size(); ++i) {
        if (std::find(runeTypeV.begin(),runeTypeV.end(),in[i]) == runeTypeV.end()) runeTypeV.push_back(in[i]);
    }
    //for each rune type 
    for (auto v : runeTypeV) {
        //create a RuneTypeNode
        root.RTN.push_back(RuneTypeNode(v)); //recall push back uses copy constructor 
        //create a finRuneTypeNode
        FRTN.push_back(finRuneTypeNode(v));
        //associated pos 
        for (int i = 0; i < in.size(); ++i) {
            if (in[i] == v && i != lead /*don't store pos for the lead*/ )root.RTN.back().pos.push_back(i); 
            if (out[i] == v) FRTN.back().pos.push_back(i);
        }

        //dis
        /*
        * matrix storing distance info used in minimum perfect matching
        * ie. for each rune type , match indices of runes on in board to indices of runes on the out board 
        */
       root.RTN.back().dis.resize(FRTN.back().pos.size(), FRTN.back().pos.size());
        //fill matrix dis (non lead runes)
        for (int i = 0; i < root.RTN.back().pos.size(); ++i) {
            for (int j = 0; j < FRTN.back().pos.size(); ++j) {
                root.RTN.back().dis(i,j) = minDis(root.RTN.back().pos[i],FRTN.back().pos[j]);
            }
        }
        //fill matrix dis (lead rune)
        if (root.RTN.back().pos.size() == FRTN.back().pos.size() - 1 /*sanity check*/) {
            for (int j = 0; j < FRTN.back().pos.size(); ++j) {
            root.RTN.back().dis(root.RTN.back().pos.size(),j) = BIG_NUM; 
            }
        }
        //compute errors 
        root.RTN.back().computeTypeErr();
        root.Err += root.RTN.back().typeErr; 

    }   
}

bool move(std::vector<int>& board /*update*/, int& i /*update*/, const int dir) {
        if (dir == NW) {
            //row > 0 && col > 0 ?
            if (i > BOARD_COL - 1 && i%BOARD_COL != 0) {
                int temp = board[i-BOARD_COL-1];  
                board[i-BOARD_COL-1] = board[i]; 
                board[i] = temp; 
                i = i-BOARD_COL-1;
                return 1; 
            }
            return 0; 
        } 
        if (dir == W) {
            //col > 0 ?
            if (i%BOARD_COL != 0) {
                int temp = board[i-1];  
                board[i-1] = board[i]; 
                board[i] = temp; 
                i = i-1;
                return 1;  
            }
            return 0; 
        }
        if (dir == SW) {
            //row < BOARD_ROW && col > 0 ?
            if (i < BOARD_COL*(BOARD_ROW-1) && i%BOARD_COL != 0) {
                int temp = board[i+BOARD_COL-1];  
                board[i+BOARD_COL-1] = board[i]; 
                board[i] = temp; 
                i = i+BOARD_COL-1; 
                return 1; 
            }
            return 0; 
        }
        if (dir == S) {
            //row < BOARD_ROW
            if (i < BOARD_COL*(BOARD_ROW-1)) {
                int temp = board[i+BOARD_COL];  
                board[i+BOARD_COL] = board[i]; 
                board[i] = temp; 
                i = i+BOARD_COL;
                return 1; 
            }
            return 0; 
        }
        if (dir == SE) {
            //row < BOARD_ROW && col < BOARD_COL 
            if (i < BOARD_COL*(BOARD_ROW-1) && (i+1)%BOARD_COL != 0) {
                int temp = board[i+BOARD_COL+1];  
                board[i+BOARD_COL+1] = board[i]; 
                board[i] = temp; 
                i = i+BOARD_COL+1; 
                return 1; 
            }
            return 0; 
        }
        if (dir == E) {
            //col < BOARD_COL 
            if ((i+1)%BOARD_COL != 0) {
                int temp = board[i+1];  
                board[i+1] = board[i]; 
                board[i] = temp; 
                i = i+1;
                return 1; 
            }
            return 0; 
        }
        if (dir == NE) {
            //row > 0 && col < BOARD_COL 
            if (i > BOARD_COL - 1 && (i+1)%BOARD_COL != 0) {
                int temp = board[i-BOARD_COL+1];  
                board[i-BOARD_COL+1] = board[i]; 
                board[i] = temp; 
                i = i-BOARD_COL+1;
                return 1; 
            }
            return 0; 
        }
        if (dir == N) {
            //row > 0
            if (i > BOARD_COL - 1) {
                int temp = board[i-BOARD_COL];  
                board[i-BOARD_COL] = board[i]; 
                board[i] = temp; 
                i = i-BOARD_COL; 
                return 1; 
            }
            return 0; 
        }
    return 0;  
}

bool compWinner (const TreeNode& i , const TreeNode& j) {
    return (i.pastErr < j.pastErr); 
}

bool compForcastedTotalErr (const TreeNode& i , const TreeNode& j) {
    return ((i.Err + i.pastErr) < (j.Err + j.pastErr)); 
}

float percenToKeep(const int size) {
    if (size < 50000) {
        return 1.0f; 
    }  else {
        return 0.15f; 
    }
}

void RuneTypeNode::computeTypeErr () const {
    Matrix<float> temp = dis;
    typeErr = 0.0f; //reset to 0 
	// Apply Munkres algorithm to matrix.
	Munkres<float> m;
	m.solve(temp);
    //computeTypeErr 
    for (int i = 0; i < pos.size(); ++i) {
        for (int j = 0; j < dis.columns(); ++j) {
            if (temp(i,j) == 0) {
                typeErr += dis(i,j); 
            }
        }
    }
} 

bool pf56_single (const std::vector<int> in , const std::vector<int> out , const int lead, const int pathWeightBound , TreeNode& solutionNode){
   TreeNode root; 
   std::vector<finRuneTypeNode> FRTN;
   preProcessor(in,out,lead,root,FRTN); 
   std::vector<TreeNode> f = {root}; //frontier 
   #ifdef MYDEBUG
   std::cout << "Init err : " << root.Err << std::endl; 
   #endif
   //BFS
    while (f.size() > 0) {
        std::vector<TreeNode> nf; //next frontier
        std::vector<TreeNode> winners; //they're all solutions to this problem , we try to pick the best one 
        for (auto v : f) {
            int dirToIgnore = 0; //direction that the lead will not explore , helps prevent going back immediately (dir is valued between [1,8] starting from NE anticlockwise)
            if (v.leadv.size() > 1 /*if the lead has moved before*/) {
                //calculation based on entry values on board (valued between [0,29])
                switch (v.leadv.back() - v.leadv[v.leadv.size()-2] /*diff between this and previous lead pos*/)
                {
                case 7: //we moved SE 
                    dirToIgnore = 1; //ignore NW
                    break;
                case 1 :
                    dirToIgnore = 2; 
                    break;
                case -5 :
                    dirToIgnore = 3; 
                    break;
                case -6 :
                    dirToIgnore = 4; 
                    break;
                case -7 :
                    dirToIgnore = 5; 
                    break;
                case -1 :
                    dirToIgnore = 6; 
                    break;
                case 5 :
                    dirToIgnore = 7; 
                    break;
                default:
                    dirToIgnore = 8; 
                    break;
                }
            } 
            //move in each dir (dir is valued between [1,8] starting from NE anticlockwise)
            for (int dir = 1; dir <= 8; ++dir) {
                if (dir == dirToIgnore) continue; //prevent going back immediately 
                nf.push_back(v); //copy construct parent . nf.back() is the current node -- the child of v
                //move
                nf.back().leadv.push_back(nf.back().leadv[nf.back().leadv.size()-1]); // f.back().leadv.back() will be the lead pos of current node after moving . now it's just a temp value to hold place .
                //invalid move
                if (!move(nf.back().board,nf.back().leadv.back(),dir)) {
                    nf.pop_back(); //remove this child
                    continue;
                }
                //update past err due to successful move 
                if (dir == 1 || dir == 3 || dir == 5 || dir == 7) {
                    nf.back().pastErr += DIAGONAL_WEIGHT;
                } else {
                    nf.back().pastErr += 1; 
                } 
                //[compute & process board error] 
                for (int typeIndex = 0; typeIndex < nf.back().RTN.size(); ++typeIndex) {
                    /*
                    * if we found target rune type node (the rune type node having the same type as key)
                    * temp.back().leadv[temp.back().leadv.size() - 2] is the current key position on current board 
                    */
                   int keyBoardIndex = nf.back().leadv[nf.back().leadv.size() - 2]; 
                    if (nf.back().RTN[typeIndex].typeName == nf.back().board[keyBoardIndex]) {
                        /*
                        * get index of key in pos vector / row in dis matrix
                        * temp.back().leadv.back() is the current lead index on on current board . Or the key position on previous board . 
                        */
                        int keyPosVIndex = std::find(nf.back().RTN[typeIndex].pos.begin(),nf.back().RTN[typeIndex].pos.end(),nf.back().leadv.back()) - nf.back().RTN[typeIndex].pos.begin();
                        //update the pos
                        nf.back().RTN[typeIndex].pos[keyPosVIndex] = keyBoardIndex;

                        //update whole row of dis where key locates 
                        for (int col = 0; col < FRTN[typeIndex].pos.size(); ++col) {
                            nf.back().RTN[typeIndex].dis(keyPosVIndex,col) = minDis(keyBoardIndex,FRTN[typeIndex].pos[col]);
                        }
                        //update type error & board error 
                        nf.back().Err -= nf.back().RTN[typeIndex].typeErr;
                        nf.back().RTN[typeIndex].computeTypeErr();
                        nf.back().Err += nf.back().RTN[typeIndex].typeErr;
                        //if solution found
                        if (nf.back().Err <= 0) {
                            winners.push_back(nf.back()); 
                            break; 
                        }
                        //if current is disqualified (although unlikely since this's greedy)
                        if (nf.back().Err > (pathWeightBound - nf.back().pastErr) /*this should be correct bound but not tight !!*/) {
                            nf.pop_back();
                            break; 
                        }
                        //else ie. current is neither a solution nor disqualified
                        break; 
                    }
                }
                //[compute & process board error] 
            }
            //at this point , we've done exploring v . Moving on to next node in frontier
        }
            //return best winner (if any)
            if (winners.size() > 0) {
                std::sort(winners.begin(),winners.end(),compWinner);
                solutionNode = winners[0]; 
                return 1; 
            } 
            //[relaxed-greedy selection]
            std::sort(nf.begin(),nf.end(),compForcastedTotalErr);
            //sizeToKeep depends on nf.size()
            const int sizeToKeep = percenToKeep(nf.size()) * nf.size(); 
            for (int i = nf.size() - 1 ; i > sizeToKeep - 1; --i) {
                nf.pop_back();
            }
            //[relaxed-greedy selection]
            //update frontier
            f = nf;    
    }
    //failed to find solution 
    return 0; 
}

/*
* runeTypeV , RTM , FRTM share same indexing 
*/
bool pf56_double (const std::vector<int> in , const std::vector<int> out , const int pathWeightBound , TreeNode& solutionNode){
    std::vector<int> runeTypeV; //contains the name of all rune types on in board 
    //initialize runeTypeV 
    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (std::find(runeTypeV.begin(),runeTypeV.end(),in[i]) == runeTypeV.end()) runeTypeV.push_back(in[i]);
    }
    
    std::vector<int> typeToIndex(NUM_RUNE_TYPES,-1); //typeToIndex[i] returns index to runeTypeV (and others) of type i . Arguments are 0,...,NUM_RUNE_TYPES-1

    std::vector<Matrix<float> > RTM; //contains dis matrices for each runetype on the initial board 
    // std::vector<Matrix<float> > FRTM; //contains dis matrices for each runetype on the out board 
    float errRef = 0; //error computed with no lead . For reference only . 
    std::vector<float> typeErrRef; //error computed with no lead . For reference only . 
    //for each rune type 
    for (int k = 0; k < runeTypeV.size(); ++k) {
        //associated pos 
        std::vector<int> inPos; //indices on in board that runeTypeV[k] occupies 
        std::vector<int> outPos; //indices on out board that runeTypeV[k] occupies 
        for (int i = 0; i < BOARD_SIZE; ++i) {
            if (in[i] == runeTypeV[k]) inPos.push_back(i); 
            if (out[i] == runeTypeV[k])  outPos.push_back(i);
        }

        typeToIndex[runeTypeV[k]] = k;

        //dis
        /*
        * matrix storing distance info used in minimum perfect matching
        * ie. for each rune type , match indices of runes on in board to indices of runes on the out board 
        * should be square matrix 
        */
       Matrix<float> dis(inPos.size(), outPos.size());
        //fill matrix dis 
        for (int i = 0; i < inPos.size(); ++i) {
            for (int j = 0; j < outPos.size(); ++j) {
                dis(i,j) = minDis(inPos[i],outPos[j]);
            }
        }
        RTM.push_back(dis);

        //compute errors 
        float typeErr = 0.0f; 
        Matrix<float> temp = dis;
        Munkres<float> m;
        m.solve(temp);
        for (int i = 0; i < inPos.size(); ++i) {
            for (int j = 0; j < outPos.size(); ++j) {
                if (temp(i,j) == 0) {
                    typeErr += dis(i,j); 
                }
            }
        }

        typeErrRef.push_back(typeErr);
        errRef += typeErr; 
    }   
    
    float minInErr = BIG_NUM; //minimum error on the in board 
    int minInErrARG = -1; //the position where minimum error occurs 
    float minOutErr = BIG_NUM; //minimum error on the out board 
    int minOutErrARG = -1; //the position where minimum error occurs 
    std::vector<float> typeOcurranceIN(runeTypeV.size(),0); //typeOcurrance[i] denotes the typeOcurrance[i]th occurance of a type (on in board)
    std::vector<float> typeOcurranceOUT(runeTypeV.size(),0); //typeOcurrance[i] denotes the typeOcurrance[i]th occurance of a type (on out board)
    for (int a = 0; a < BOARD_SIZE; ++a) {

        {
            //[in]
            int i = typeToIndex[in[a]]; //index to runeTypeV (and others)
            typeOcurranceIN[i]++;
            Matrix<float> inDis = RTM[i];
            //modify a row 
            for (int col = 0; col < inDis.columns(); ++col) {
                inDis(typeOcurranceIN[i]-1,col) = BIG_NUM; 
            }
            //compute new type err 
            float inTypeErr = 0; 
            Matrix<float> temp = inDis;
            Munkres<float> m; // Munkres algorithm
            m.solve(temp); // Apply Munkres algorithm to matrix.
            for (int x = 0; x < temp.rows(); ++x) {
                if (x == typeOcurranceIN[i]-1) continue; //ignore the row since it denotes the lead 
                for (int y = 0; y < temp.columns(); ++y) {
                    if (temp(x,y) == 0) inTypeErr += inDis(x,y); 
                }
            }
            //update minInErr & minInErrARG
            if (errRef - typeErrRef[i] + inTypeErr < minInErr) {
                minInErr = errRef - typeErrRef[i] + inTypeErr;
                minInErrARG = a; 
            }
            //[in]
        }

        //[out]
        int i = typeToIndex[out[a]]; //index to runeTypeV (and others)
        typeOcurranceOUT[i]++;
        Matrix<float> outDis = RTM[i];
        //modify a col 
        for (int row = 0; row < outDis.rows(); ++row) {
            outDis(row,typeOcurranceOUT[i]-1) = BIG_NUM; 
        }
        //compute new type err 
        float outTypeErr = 0; 
        Matrix<float> temp = outDis;
        Munkres<float> m; // Munkres algorithm 
        m.solve(temp); // Apply Munkres algorithm to matrix.
        for (int y = 0; y < temp.columns(); ++y) {
            if (y == typeOcurranceOUT[i]-1) continue; //ignore the col since it denotes the lead 
            for (int x = 0; x < temp.rows(); ++x) {
                if (temp(x,y) == 0) outTypeErr += outDis(x,y); 
            }
        }
        //update minInErr & minInErrARG
        if (errRef - typeErrRef[i] + outTypeErr < minOutErr) {
            minOutErr = errRef - typeErrRef[i] + outTypeErr;
            minOutErrARG = a; 
        }
        //[out]

    }

    TreeNode inSol; //solution by starting from in board 
    std::thread t1 (pf56_single,in,out,minInErrARG,pathWeightBound,std::ref(inSol));
    TreeNode outSol; //solution by starting from out board 
    std::thread t2 (pf56_single,out,in,minOutErrARG,pathWeightBound,std::ref(outSol));
    t1.join();
    t2.join();

    if (inSol.pastErr < outSol.pastErr) {
        solutionNode = inSol; 
        return 1;
    } else if (inSol.pastErr > outSol.pastErr) {
        std::reverse(outSol.leadv.begin(),outSol.leadv.end()); 
        solutionNode = outSol;
        return 1;
    } else if (inSol.pastErr != 0 /*they're equal too*/) {
        solutionNode = inSol; //arbitrarily pick one 
        return 1;
    } else {
        return 0; 
    }
}

#endif 
