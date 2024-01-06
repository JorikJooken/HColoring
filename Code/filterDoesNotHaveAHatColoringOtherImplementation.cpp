#include <bits/stdc++.h>

// Unsafe because no defined behaviour if character = 0. ctz and clz work with 32 bit numbers.
#define unsafePrev(character, current) (__builtin_ctz(character) - current >= 0 ? -1 : current -__builtin_clz((character) << (32 - current)) - 1)

#define prev(character,current) (character ? unsafePrev(character,current) : -1)

using namespace std;

const int nb_bits=64;
#define MAXVERTICES 64

int getNumberOfVertices(string graphString) 
{
	if(graphString.size() == 0){
        printf("Error: String is empty.\n");
        abort();
    }
    else if((graphString[0] < 63 || graphString[0] > 126) && graphString[0] != '>') {
    	printf("Error: Invalid start of graphstring.\n");
    	abort();
    }

	int index = 0;
	if (graphString[index] == '>') { // Skip >>graph6<< header.
		index += 10;
	}

	if(graphString[index] < 126) { // 0 <= n <= 62
		return (int) graphString[index] - 63;
	}

	else if(graphString[++index] < 126) { 
		int number = 0;
		for(int i = 2; i >= 0; i--) {
			number |= (graphString[index++] - 63) << i*6;
		}
		return number;
	}

	else if (graphString[++index] < 126) {
		int number = 0;
		for (int i = 5; i >= 0; i--) {
			number |= (graphString[index++] - 63) << i*6;
		}
		return number;
	}

	else {
		printf("Error: Format only works for graphs up to 68719476735 vertices.\n");
		abort();
	}
}

void loadGraph(string graphString, int numberOfVertices, long long adjacencyList[]) {
	int startIndex = 0;
	if (graphString[startIndex] == '>') { // Skip >>graph6<< header.
		startIndex += 10;
	}
	if (numberOfVertices <= 62) {
		startIndex += 1;
	}
	else if (numberOfVertices <= MAXVERTICES) {
		startIndex += 4;
	}
	else {
		printf("Error: Program can only handle graphs with %d vertices or fewer.\n",MAXVERTICES);
		abort();
	}

	for (int vertex = 0; vertex < numberOfVertices; vertex++) { //Initialize adjacencyList.
		adjacencyList[vertex] = 0;
	}

	int currentVertex = 1;
	int sum = 0; 
	for (int index = startIndex; index<graphString.size(); index++) {
		int i;
		for (i = prev(graphString[index] - 63, 6); i != -1; i = prev(graphString[index] - 63, i)) {
			while(5-i+(index-startIndex)*6 - sum >= 0) {
				sum += currentVertex;
				currentVertex++;
			}
			sum -= --currentVertex;
			int neighbour = 5-i+(index - startIndex)*6 - sum;
            adjacencyList[currentVertex]|=(1LL<<neighbour);
            adjacencyList[neighbour]|=(1LL<<currentVertex);
		}
	}
}

#define left_shifted(n,x) ((n)<<(x))
#define set_bit(n,i) ((n)|=left_shifted(1LL,i))
#define count_bits(n) (__builtin_popcountll((n)))
//#define index_rightmost_bit(n) (count_bits((n)^(n-1))-1)
#define index_rightmost_bit(n) (__builtin_ctzll(n))
#define rightmost_bit_removed(n) ((n)&((n)-1))
#define traverse_through_neighbors_nest1(x,neighbor) for(long long (neighbor)=index_rightmost_bit((x)),DESTROYED_MASK1=(x); (DESTROYED_MASK1)!=0; (DESTROYED_MASK1)=rightmost_bit_removed((DESTROYED_MASK1)),(neighbor)=index_rightmost_bit((DESTROYED_MASK1)))
#define traverse_through_neighbors_nest2(x,neighbor) for(long long (neighbor)=index_rightmost_bit((x)),DESTROYED_MASK2=(x); (DESTROYED_MASK2)!=0; (DESTROYED_MASK2)=rightmost_bit_removed((DESTROYED_MASK2)),(neighbor)=index_rightmost_bit((DESTROYED_MASK2)))

int n;
long long adjacencyList[MAXVERTICES];

#define nbColors 7
int AArray[3][4];
int color[MAXVERTICES][MAXVERTICES];
int nbColorsPossible[MAXVERTICES][MAXVERTICES];
bool colorIsPossible[MAXVERTICES][MAXVERTICES][nbColors];

int ctr=0;
int nbEdges=0;

bool canBeColoredRec(int edgeNumber)
{
    if(edgeNumber==nbEdges)
    {
        return true;
    }
    // search for edge with least number of possible colors
    int from=-1;
    int to=-1;
    int leastNumber=nbColors+1;
    for(int i=0; i<n; i++)
    {
        traverse_through_neighbors_nest1(adjacencyList[i],neigh)
        {
            if(color[i][neigh]==-1)
            {
                if(nbColorsPossible[i][neigh]<leastNumber)
                {
                    leastNumber=nbColorsPossible[i][neigh];
                    from=i;
                    to=neigh;
                }
            }
        }
    }

    for(int currColor=0; currColor<nbColors; currColor++)
    {
        if(!colorIsPossible[from][to][currColor]) continue;
        /*cout << "Iteration " << edgeNumber << endl;
        cout << "Assigning color " << currColor << " to edge between " << from << " and " << to << endl;*/
        color[from][to]=color[to][from]=currColor;

        vector<int> colorsAdjacentToFrom;
        // which colors are adjacent to "from"
        traverse_through_neighbors_nest1(adjacencyList[from],neighbor)
        {
            if(color[from][neighbor]!=-1)
            {
                colorsAdjacentToFrom.push_back(color[from][neighbor]);
            }
        }
        // can be mapped to vertex x
        vector<bool> canBeMapped(3,true);
        for(int i=0; i<3; i++)
        {
            bool hasAll=true;
            for(int col : colorsAdjacentToFrom)
            {
                bool isIn=false;
                for(int j=0; j<4 && !isIn; j++)
                {
                    if(col==AArray[i][j]) isIn=true;
                }
                if(!isIn)
                {
                    hasAll=false;
                    break;
                }       
            }
            if(!hasAll) canBeMapped[i]=false;
        }
        // check which colors might be possible for edges adjacent to from
        vector<bool> perhapsPossibleColor(nbColors,false);
        for(int i=0; i<3; i++)
        {
            if(canBeMapped[i])
            {
                for(int j=0; j<4; j++)
                {
                    perhapsPossibleColor[AArray[i][j]]=true;
                }
            }
        }
        
        vector< pair< pair<int, int>, int> > changedFromTrueToFalse; 
        // make currColor impossible for neighbor        
        traverse_through_neighbors_nest1(adjacencyList[from],neighbor)
        {
            if(neighbor==to) continue;
            if(color[from][neighbor]!=-1) continue;
            for(int k=0; k<nbColors; k++)
            {
                if(colorIsPossible[from][neighbor][k])
                {
                    if(k==currColor || !perhapsPossibleColor[k])
                    {
                        colorIsPossible[from][neighbor][k]=colorIsPossible[neighbor][from][k]=false;
                        nbColorsPossible[from][neighbor]-=1;
                        nbColorsPossible[neighbor][from]-=1;
                        changedFromTrueToFalse.push_back(make_pair(make_pair(from,neighbor),k));
                    }
                }
            }
        }

        swap(from,to);
        colorsAdjacentToFrom.clear();
        // which colors are adjacent to "from"
        traverse_through_neighbors_nest1(adjacencyList[from],neighbor)
        {
            if(color[from][neighbor]!=-1)
            {
                colorsAdjacentToFrom.push_back(color[from][neighbor]);
            }
        }
        // can be mapped to vertex x
        canBeMapped.assign(3,true);
        for(int i=0; i<3; i++)
        {
            bool hasAll=true;
            for(int col : colorsAdjacentToFrom)
            {
                bool isIn=false;
                for(int j=0; j<4 && !isIn; j++)
                {
                    if(col==AArray[i][j]) isIn=true;
                }
                if(!isIn)
                {
                    hasAll=false;
                    break;
                }       
            }
            if(!hasAll) canBeMapped[i]=false;
        }
        // check which colors might be possible for edges adjacent to from
        perhapsPossibleColor.assign(nbColors,false);
        for(int i=0; i<3; i++)
        {
            if(canBeMapped[i])
            {
                for(int j=0; j<4; j++)
                {
                    perhapsPossibleColor[AArray[i][j]]=true;
                }
            }
        }
        // make currColor impossible for neighbor        
        traverse_through_neighbors_nest1(adjacencyList[from],neighbor)
        {
            if(neighbor==to) continue;
            if(color[from][neighbor]!=-1) continue;
            for(int k=0; k<nbColors; k++)
            {
                if(colorIsPossible[from][neighbor][k])
                {
                    if(k==currColor || !perhapsPossibleColor[k])
                    {

                        colorIsPossible[from][neighbor][k]=colorIsPossible[neighbor][from][k]=false;
                        nbColorsPossible[from][neighbor]-=1;
                        nbColorsPossible[neighbor][from]-=1;
                        changedFromTrueToFalse.push_back(make_pair(make_pair(from,neighbor),k));
                    }
                }
            }
        }
        swap(from,to);
        if(canBeColoredRec(edgeNumber+1)) return true;
        
        for(pair< pair<int, int>, int> pa : changedFromTrueToFalse)
        {
            pair<int, int> fromAndTo=pa.first;
            int from=fromAndTo.first;
            int to=fromAndTo.second;
            int col=pa.second;
            colorIsPossible[from][to][col]=colorIsPossible[to][from][col]=true;
            nbColorsPossible[from][to]+=1;
            nbColorsPossible[to][from]+=1;
        }
        color[from][to]=color[to][from]=-1;
    }
    return false;
}

bool canBeColored()
{
    for(int i=0; i<n; i++)
    {
        for(int j=0; j<n; j++)
        {
            color[i][j]=-1;
            nbColorsPossible[i][j]=nbColors;
            for(int k=0; k<nbColors; k++)
            {
                colorIsPossible[i][j][k]=true;
            }
        }
    }
    return canBeColoredRec(0);
}

// Expects as input a list of simple connected 4-regular graphs in graph6 format (one graph per line)
// It will output all graphs that are not $\hat{A}$-colorable
int main()
{
    AArray[0][0]=0;
    AArray[0][1]=1;
    AArray[0][2]=2;
    AArray[0][3]=3;

    AArray[1][0]=2;
    AArray[1][1]=3;
    AArray[1][2]=4;
    AArray[1][3]=5;

    AArray[2][0]=1;
    AArray[2][1]=4;
    AArray[2][2]=5;
    AArray[2][3]=6;

    ios::sync_with_stdio(false);
    cin.tie(0);
    long long nb_graphs_read_from_input=0;
    string line;
    while(getline(cin,line))
    {
        //line+="\n";
        nb_graphs_read_from_input++;
        n = getNumberOfVertices(line);
        loadGraph(line,n,adjacencyList);
        nbEdges=0;
        for(int i=0; i<n; i++)
        {
            nbEdges+=count_bits(adjacencyList[i]);
        }
        nbEdges/=2;
        if(!canBeColored()) cout << line << endl;
    }
    return 0;
}
