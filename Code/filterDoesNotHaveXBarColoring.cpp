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

int AArray[3][4];
int color[MAXVERTICES][MAXVERTICES];
int numberIncidentEdgesColored[MAXVERTICES];

int ctr=0;

bool canBeColoredRec(long long unvisited)
{
    if(unvisited==0)
    {
        return true;
    }
    // search for unvisited vertex with most colored incident edges
    pair<int, int> maxMe=make_pair(-1,-1);
    traverse_through_neighbors_nest1(unvisited,v)
    {
        maxMe=max(maxMe,make_pair(numberIncidentEdgesColored[v],(int)v));
    }
    int v=maxMe.second;
    /*if(ctr>100) return false;
    cerr << unvisited << " " << v << endl;
    ctr++;*/
    for(int assignment=0; assignment<3; assignment++)
    {
        vector<int> pos(4,0);
        for(int i=1; i<4; i++) pos[i]=i;
        do
        {
            int edge_ctr=0;
            vector<int> oldColor(4,0);
            bool ok=true;
            traverse_through_neighbors_nest1(adjacencyList[v],neigh)
            {
                oldColor[edge_ctr]=color[v][neigh];
                //cerr << assignment << " " << pos[edge_ctr] << endl;
                int newColor=AArray[assignment][pos[edge_ctr]];
                if(color[v][neigh]!=-1 && color[v][neigh]!=newColor) ok=false;
                if(color[v][neigh]==-1) numberIncidentEdgesColored[neigh]++;
                color[v][neigh]=color[neigh][v]=newColor;
                edge_ctr++;
            }
            if(ok)
            {
                long long newUnvisited=(unvisited^(1LL<<v));
                if(canBeColoredRec(newUnvisited)) return true;
            }
            edge_ctr=0;
            traverse_through_neighbors_nest1(adjacencyList[v],neigh)
            {
                color[v][neigh]=color[neigh][v]=oldColor[edge_ctr];
                if(color[v][neigh]==-1) numberIncidentEdgesColored[neigh]--;
                edge_ctr++;
            }
        }while(next_permutation(pos.begin(),pos.end()));
    }
    return false;
}

bool canBeColored()
{
    // make edge ids
    for(int i=0; i<n; i++)
    {
        numberIncidentEdgesColored[i]=0;
        for(int j=0; j<n; j++)
        {
            color[i][j]=-1;
        }
    }
    long long unvisited=(1LL<<n)-1;
    //cerr << "unvisited " << unvisited << endl;
    return canBeColoredRec(unvisited);
}

// Expects as input a list of simple connected 4-regular graphs in graph6 format (one graph per line)
// It will output all graphs that are not $\bar{X}$-colorable
int main()
{
    AArray[0][0]=0;
    AArray[0][1]=1;
    AArray[0][2]=2;
    AArray[0][3]=3;

    AArray[1][0]=2;
    AArray[1][1]=4;
    AArray[1][2]=5;
    AArray[1][3]=6;

    AArray[2][0]=3;
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
        if(!canBeColored()) cout << line << endl;
    }
    return 0;
}
