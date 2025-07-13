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
    unordered_map<int, User> users;
    unordered_map<int, vector<int>> graph;
    unordered_map<int, vector<int>> friendRequest;
    unordered_map<int, unordered_set<int>> blockedUsers; // renamed
    unordered_map<int, int> mutualCount;
    unordered_map<int, int> parent;

    void addUser(User& user)
    {
        users[user.id] = user;
    }

    void addFriend(int userOne, int userTwo)
    {
        if (find(graph[userOne].begin(), graph[userOne].end(), userTwo) == graph[userOne].end())
        {
            graph[userOne].push_back(userTwo);
        }
        if (find(graph[userTwo].begin(), graph[userTwo].end(), userOne) == graph[userTwo].end())
        {
            graph[userTwo].push_back(userOne);
        }
    }

    void displayFriends(int userId)
    {
        if (graph.find(userId) == graph.end() || graph[userId].empty())
        {
            cout << users[userId].name << " has no friends\n";
            return;
        }
        cout << "Friends of: " << users[userId].name << ": ";
        for (int friendId : graph[userId])
        {
            cout << users[friendId].name << " ";
        }
        cout << "\n";
    }

    void sendFriendRequest(int senderId, int receiverId)
    {
        if (find(graph[senderId].begin(), graph[senderId].end(), receiverId) != graph[senderId].end())
        {
            cout << users[senderId].name << " and " << users[receiverId].name << " are already friends\n";
            return;
        }
        if (find(friendRequest[receiverId].begin(), friendRequest[receiverId].end(), senderId) != friendRequest[receiverId].end())
        {
            cout << "Friend request already sent to " << users[receiverId].name << "!\n";
            return;
        }
        friendRequest[receiverId].push_back(senderId);
        cout << users[senderId].name << " sent a friend request to " << users[receiverId].name << "\n";
    }

    void acceptFriendRequest(int senderId, int receiverId)
    {
        auto& request = friendRequest[receiverId];
        auto it = find(request.begin(), request.end(), senderId);

        if (it == request.end())
        {
            cout << users[receiverId].name << " has no friend request from " << users[senderId].name << "\n";
            return;
        }

        addFriend(senderId, receiverId);
        request.erase(it);
        cout << users[receiverId].name << " accepted friend request from " << users[senderId].name << "\n";
    }

    void rejectFriendRequest(int senderId, int receiverId)
    {
        auto& request = friendRequest[receiverId];
        auto it = find(request.begin(), request.end(), senderId);

        if (it == request.end())
        {
            cout << users[senderId].name << " has not sent friend request to " << users[receiverId].name << "\n";
            return;
        }
        request.erase(it);
        cout << users[receiverId].name << " declined friend request from " << users[senderId].name << "\n";
    }

    void showPendingFriendRequest(int userId)
    {
        if (friendRequest[userId].empty())
        {
            cout << users[userId].name << " has no pending friend requests.\n";
            return;
        }
        cout << "All pending friend requests for " << users[userId].name << ": ";
        for (int pendingFriendRequestId : friendRequest[userId])
        {
            cout << users[pendingFriendRequestId].name << " ";
        }
        cout << "\n";
    }

    void blockUser(int blockerId, int blockId)
    {
        if (blockedUsers[blockerId].find(blockId) == blockedUsers[blockerId].end())
        {
            blockedUsers[blockerId].insert(blockId);
            cout << users[blockerId].name << " has blocked " << users[blockId].name << "\n";
        }
        else
        {
            cout << users[blockerId].name << " already blocked " << users[blockId].name << "\n";
        }
    }

    void unblockUser(int blockerId, int blockId)
    {
        if (blockedUsers[blockerId].find(blockId) != blockedUsers[blockerId].end())
        {
            blockedUsers[blockerId].erase(blockId);
            cout << users[blockerId].name << " has unblocked " << users[blockId].name << "\n";
        }
        else
        {
            cout << users[blockerId].name << " has not blocked " << users[blockId].name << "\n";
        }
    }

    bool cmp(pair<int,int>& a, pair<int,int>& b)
    {
        return a.second > b.second;
    }

    vector<int> friendSuggestion(int userId, vector<int>& suggestedFriends)
    {
        mutualCount.clear();
        unordered_set<int> visited;
        visited.insert(userId);

        queue<int> q;
        q.push(userId);

        while (!q.empty())
        {
            int current = q.front();
            q.pop();

            for (int friendId : graph[current])
            {
                if (visited.find(friendId) == visited.end())
                {
                    visited.insert(friendId);
                    q.push(friendId);
                }
                // Count mutual friends of user's friends
                if (friendId != userId)
                {
                    for (int mutual : graph[friendId])
                    {
                        if (mutual != userId && visited.find(mutual) == visited.end())
                        {
                            mutualCount[mutual]++;
                        }
                    }
                }
            }
        }

        vector<pair<int,int>> sortedSuggestion(mutualCount.begin(), mutualCount.end());
        sort(sortedSuggestion.begin(), sortedSuggestion.end(), [&](auto &a, auto &b) {
            return a.second > b.second;
        });

        for (auto &p : sortedSuggestion)
        {
            suggestedFriends.push_back(p.first);
        }

        return suggestedFriends;
    }

    vector<int> createTempArray(const string& pattern)
    {
        vector<int> lps(pattern.size());
        int index = 0;
        for (int i = 1; i < (int)pattern.size(); )
        {
            if (pattern[i] == pattern[index])
            {
                lps[i] = index + 1;
                index++;
                i++;
            }
            else
            {
                if (index != 0)
                {
                    index = lps[index - 1];
                }
                else
                {
                    lps[i] = 0;
                    i++;
                }
            }
        }
        return lps;
    }

    bool patternSearching(const string& pattern, const string& text)
    {
        vector<int> lps = createTempArray(pattern);
        int i = 0, j = 0;

        while (i < (int)text.size())
        {
            if (text[i] == pattern[j])
            {
                i++;
                j++;
                if (j == (int)pattern.size())
                    return true;
            }
            else
            {
                if (j != 0)
                    j = lps[j - 1];
                else
                    i++;
            }
        }
        return false;
    }

    void searchFriendList(int userId, const string& userName)
    {
        if (graph.find(userId) == graph.end() || graph[userId].empty())
        {
            cout << users[userId].name << " has no friends\n";
            return;
        }

        bool found = false;
        string username = userName;
        transform(username.begin(), username.end(), username.begin(), ::tolower);

        for (int friendId : graph[userId])
        {
            string friendName = users[friendId].name;
            string lowerFriendName = friendName;
            transform(lowerFriendName.begin(), lowerFriendName.end(), lowerFriendName.begin(), ::tolower);

            if (patternSearching(username, lowerFriendName))
            {
                found = true;
                cout << "Name: " << friendName << "\n";
            }
        }

        if (!found)
            cout << "No match found\n";
    }
};

int main()
{
    fastIO();

    SocialNetwork Sn;
    int numberOfUsers, numberOfFriendships;
    cout << "Number of Users: ";
    cin >> numberOfUsers;

    for (int i = 0; i < numberOfUsers; i++)
    {
        User user;
        cout << "Id and Name: ";
        cin >> user.id >> user.name;
        Sn.addUser(user);
    }

    cout << "Number of Friendships: ";
    cin >> numberOfFriendships;

    for (int i = 0; i < numberOfFriendships; i++)
    {
        int userOne, userTwo;
        cin >> userOne >> userTwo;

        if (Sn.users.find(userOne) == Sn.users.end() || Sn.users.find(userTwo) == Sn.users.end())
        {
            cout << "Skipping invalid friendship!\n";
            continue;
        }
        Sn.addFriend(userOne, userTwo);
    }

    cout << "\nDisplaying friend list:\n";
    for (auto& u : Sn.users)
    {
        Sn.displayFriends(u.first);
    }

    // Search Friend
    int searchUserId;
    cout << "\nSearch User Id: ";
    cin >> searchUserId;
    cin.ignore();
    string searchUserName;
    cout << "Search User Name: ";
    getline(cin, searchUserName);
    Sn.searchFriendList(searchUserId, searchUserName);

    // Friend Request
    int senderId, receiverId;
    cout << "\nSend Friend Request: Enter senderId receiverId: ";
    cin >> senderId >> receiverId;
    Sn.sendFriendRequest(senderId, receiverId);

    cout << "\nAccept Friend Request: Enter senderId receiverId: ";
    cin >> senderId >> receiverId;
    Sn.acceptFriendRequest(senderId, receiverId);

    // Block & Unblock
    int blockerId, blockId;
    cout << "\nBlock User: Enter blockerId blockId: ";
    cin >> blockerId >> blockId;
    Sn.blockUser(blockerId, blockId);

    cout << "\nUnblock User: Enter blockerId blockId: ";
    cin >> blockerId >> blockId;
    Sn.unblockUser(blockerId, blockId);

    // Friend Suggestions
    vector<int> suggestedFriends;
    cout << "\nFriend suggestions for user 1:\n";
    Sn.friendSuggestion(1, suggestedFriends);
    for (auto i : suggestedFriends)
    {
        cout << i << " ";
    }
    cout << "\n";

    return 0;
}
