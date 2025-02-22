#include <bits/stdc++.h>
#define ll long long
#define pb push_back
#define ff first
#define ss second
const int mx = 100005;
using namespace std;

void fastIO()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);
}

struct User{
public:
    int id;
    string name;
};

class SocialNetwork
{
    public:
    unordered_map<int,User>users;
    unordered_map<int,vector<int>>graph;
    unordered_map<int,vector<int>>friendRequest;
    unordered_map<int,unordered_set<int>>blockUser;
    unordered_map<int,int>mutualCount;
    unordered_map<int, int> parent; 


    void addUser(User& user)
    {
        users[user.id] = user;
    }

    void addFriend(int userOne,int userTwo)
    {
        if(find(graph[userOne].begin(),graph[userOne].end(),userTwo) == graph[userOne].end())
        {
            graph[userOne].push_back(userTwo);
        }
        if(find(graph[userTwo].begin(),graph[userTwo].end(),userOne) == graph[userTwo].end())
        {
            graph[userTwo].push_back(userOne);
        }
    }

    void displayFriends(int userId)
    {
        if(graph.find(userId) == graph.end() || graph[userId].empty())
        {
            cout<<users[userId].name<<" have no friends\n";
        }

        cout<<"Friends of : "<<users[userId].name<<" : ";

        for(int friendId : graph[userId])
        {
            cout<<users[friendId].name<<" ";
        }
        cout<<"\n";
    }

    void sendFriendRequest(int senderId,int receiverId)
    {
        if(find(graph[senderId].begin(),graph[senderId].end(),receiverId) != graph[senderId].end())
        {
            cout<<users[senderId].name<<" "<<" and "<<users[receiverId].name<<" already friends\n";
            return;
        }
        if(find(friendRequest[receiverId].begin(), friendRequest[receiverId].end(), senderId) != friendRequest[receiverId].end())
        {
            cout << "Friend request already sent to " << users[receiverId].name << "!\n";
            return;
        }
        else{
            friendRequest[receiverId].push_back(senderId);//friendRequest[2] = 1
            cout<<users[senderId].name<<"sent a friend request to"<<users[receiverId].name << "\n";
        }
    }

    void acceptFriendRequest(int senderId,int receiverId)//Ajoy=1 request rahat=2
    {
        auto &request = friendRequest[receiverId];
        auto it = find(request.begin(),request.end(),senderId);

        if(it == friendRequest[receiverId].end())
        {
            cout<<users[receiverId].name<<" doesn't send friend request "<<users[senderId].name<<"\n";
            return;
        }

        graph[receiverId].push_back(senderId);
        graph[senderId].push_back(receiverId);

        friendRequest[receiverId].erase(it);
        cout<<users[receiverId].name<<" accept friend request from "<<users[senderId].name<<"\n";
    }

    void rejectFriendRequest(int senderId,int receiverId)
    {
        auto& request = friendRequest[receiverId];
        auto it = find(request.begin(),request.end(),senderId);

        if(it == friendRequest[receiverId].end())
        {
            cout<<users[senderId].name<<" doesn't send friend request\n";
            return;
        }
        friendRequest[receiverId].erase(it);
        cout<<users[receiverId].name<<" decline friend request of "<<users[senderId].name<<"\n";
    }

    void showPendingFriendRequest(int userId)
    {
        if(friendRequest[userId].empty())
        {
            cout << users[userId].name << " has no pending friend requests.\n";
            return;
        }
        cout<<"all pending request from : ";
        for(int pendingFriendRequestId : friendRequest[userId])
        {
            cout<<users[pendingFriendRequestId].name<<" ";
        }
        cout<<"\n";
    }

    void blockUser(int blockerId,int blockId)//1    2
    {
        if(find(blockUser[blockerId].begin(),blockUser[blockerId].end(),blockId) == blockUser[blockerId].end())
        {
            blockUser[blockerId].insert(blockId);
            cout<<users[blockerId].name<<" has blocked "<< users[blockId].name<<"\n";
        }
    }
    void unblockUser(int blockerId,int blockId)//1   2
    {
        if(find(blockUser[blockerId].begin(),blockUser[blockerId].end(),blockId) != blockUser[blockerId].end())
        {
            blockUser[blockerId].erase(blockId);
            cout << users[blockerId].name << " has unblocked " << users[blockId].name<<"\n";
        }
        else
        {
            cout << users[blockerId].name << " has not blocked " << users[blockId].name <<"\n";
        }
    }

    bool cmp(pair<int,int>&a,pair<int,int>&b)
    {
        if(a.second > b.second)
        {
            return true;
        }
        return false;
    }

    vector<int> friendSuggestion(int userId,vector<int>&suggestedFriends)//debug korte hobe
    {
        queue<int>q;
        unordered_set<int>visited;
        visited.insert(userId);
        q.push(userId);

        while(!q.empty())
        {
            int current = q.front();
            q.pop();
            for(int friendId : graph[current])//1 -> 2,3
            {
                if(friendId == userId)//problem
                {
                    continue;
                }
                for(int mutual : graph[friendId])
                {
                    if(mutual != userId && visited.find(mutual) == visited.end())
                    {
                        mutualCount[mutual]++;///
                    }
                }
            }
        }

        vector<pair<int,int>>SortedSuggestion(mutualCount.begin(),mutualCount.end());
        sort(SortedSuggestion.begin(),SortedSuggestion.end(),cmp);

        for(auto &p : SortedSuggestion)
        {
            suggestedFriends.push_back(p.first);
        }
        return suggestedFriends;
    }

    
    void init()
    {
        
    }

    void unionFind(int userOne,int userTwo)
    {
        init();
        /*

        find(u);//kon set a ache
        union(u,v);//duijon friend k same set e niye ashbe
        makeSet(u);//indiviual manush k set e niye asbe
        IsSameSet(u,v);//is friend or not

        */
    }

    vector<int>createTempArray(string pattern)
    {
        vector<int>lps(pattern.size());
        int index = 0;
        for(int i = 1 ; i < pattern.size() ; )
        {
            if(pattern[i] == pattern[index])
            {
                lps[i] = index + 1;
                index++;
                i++;
            }
            else
            {
                if(index != 0)
                {
                    index = lps[index - 1];
                }
                else
                {
                    lps[i] = index;
                    i++;
                }
            }
        }
        return lps;
    }

    bool patternSearching(string& pattern,string& text)//O(n + m)  trie use korbo
    {
        bool flag = false;
        vector<int>lps = createTempArray(pattern);//
        int i = 0,j = 0;
        while(i < text.size())
        {
            if(text[i] == pattern[j])
            {
                i++;
                j++;
            }
            else
            {
                if(j != 0)
                {
                    j = lps[j-1];
                }
                else
                {
                    i++;
                }
            }
            if(j == pattern.size())
            {
                flag = true;
                return true;
            }
        }

        if(flag == false)
        {
            return false;
        }
    }

    void searchFriendList(int userId,string& userName)//1
    {
        if(graph.find(userId) == graph.end() || graph[userId].empty())
        {
            cout<<users[userId].name<<" has no friends\n";
            return;
        }
        bool found = false;
        string username = userName;// di
        transform(username.begin(),username.end(),username.begin(),::tolower);

        for(int friendId : graph[userId])//2 : graph[1]
        {
            string friendName = users[friendId].name;//
            string lowerFriendName = friendName;//
            transform(lowerFriendName.begin(),lowerFriendName.end(),lowerFriendName.begin(),::tolower);

            if(patternSearching(username,lowerFriendName))//KMP algorithm
            {
                found = true;
                cout<<"Name : "<<friendName<<"\n";//all name show korbe
            }
        }
        if(!found)
        {
            cout<<"No match found\n";
        }
    }
};

int main()
{
    fastIO();

	SocialNetwork Sn;
    int numberOfUsers,numberOfFriendships;
    cin>>numberOfUsers;

    for(int i = 0 ; i < numberOfUsers ; i++)
    {
        User user;
        cout<<"Id and Name : ";
        cin>>user.id;
        cin>>user.name;
        Sn.addUser(user);
    }

    cout<<"Number of FriendShips : ";
    cin>>numberOfFriendships;

    for(int i = 0 ; i < numberOfFriendships; i++)
    {
        int userOne,userTwo;
        cin>>userOne>>userTwo;

        if(Sn.users.find(userOne) == Sn.users.end() || Sn.users.find(userTwo) == Sn.users.end())
        {
            cout<<"Skip Those friends!\n";
            continue;
        }
        Sn.addFriend(userOne,userTwo);
    }

    cout<<"Display friendlist : \n";

    unordered_map<int,bool>visited;
    for(auto i : Sn.users)
    {
        int id = i.first;
        Sn.displayFriends(id);
    }

    //search friend
    int searchUserId;
    cout<<"Search User Id : ";
    cin>>searchUserId;
    cin.ignore();
    string searchUserName;
    cout<<"Search User Name : ";
    getline(cin,searchUserName);
    Sn.searchFriendList(searchUserId,searchUserName);

    //friend request
    int senderId,receiverId;
    cin>>senderId>>receiverId;
    Sn.sendFriendRequest(senderId,receiverId);

    Sn.acceptFriendRequest(senderId,receiverId);


    int blockerId,blockId;
    cin>>blockerId>>blockId;
    Sn.blockUser(blockerId,blockId);
    Sn.unblockUser(blockerId,blockId);


    vector<int> suggestedFriends;
    Sn.friendSuggestion(1,suggestedFriends);
    for(auto i : suggestedFriends)
    {
        cout<<i<<" ";
    }

    //same group e ache kina nay eita determine korar jonno unionFind function use okra hoi..
    int friendOne,friendTwo;
    cin>>friendOne>>friendTwo;
    Sn.unionFind(friendOne,friendTwo);


    //post,comment
}