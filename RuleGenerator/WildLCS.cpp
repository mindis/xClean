//
// Created by Wenbo Tao on 1/25/17.
//

#include "WildLCS.h"
using namespace std;

WildLCS::WildLCS(vector<string> s, vector<int> w)
		: RuleGenerator(s, w)
{
	//build trie
	trie.clear(), trie.resize(1);
	contain_word.clear(), contain_word.push_back("");

	for (int i = 0; i < n; i ++)
		for (string s : tokens[i])
		{
			if (s.size() < Common::LHS_SIZE_LIM)
				continue;

			//insert s into trie
			int cur_node = 0;
			for (int j = 0; j < (int) s.size(); j ++)
			{
				if (! trie[cur_node].count(s[j]))
				{
					trie[cur_node][s[j]] = (int) trie.size();
					trie.push_back(unordered_map<char, int>());
					contain_word.push_back("");
				}
				cur_node = trie[cur_node][s[j]];
			}
			contain_word[cur_node] = s;
		}
}

vector<t_rule> WildLCS::gen_rules()
{
	unordered_set<t_rule, rule_hash> rules;
	vector<unordered_set<pair<int, int>, pairii_hash>> used(trie.size());
	vector<pair<int, pair<int, int>>> H;

	for (int i = 0; i < n; i ++)
	{
		string cur_string = "";
		vector<int> token_id;

		for (int j = 0; j < (int) tokens[i].size(); j ++)
			for (int k = 0; k < (int) tokens[i][j].size(); k ++)
			{
				cur_string += tokens[i][j][k];
				token_id.push_back(j);
			}

//		for (auto j = 0; j < cur_string.size(); j ++)
//			token_id.push_back(1);

		//traversal
		H.clear();
		for (auto j = 0; j < trie.size(); j ++)
			used[j].clear();
		H.emplace_back(0, make_pair(0, 0));
		used[0].insert(make_pair(0, 0));
		for (auto j = 0; j < cur_string.size(); j ++)
		{
			vector<pair<int, pair<int, int>>> cur_H = H;
			for (auto cp : cur_H)
			{
				int node = cp.first;
				int st = cp.second.first;
				int en = cp.second.second;

				//include current character
				if (trie[node].count(cur_string[j]))
				{
					int new_node = trie[node][cur_string[j]];
					int new_st = (st == 0 ? token_id[j] + 1 : st);
					int new_en = token_id[j] + 1;
					if (! used[new_node].count(make_pair(new_st, new_en)))
					{
						H.emplace_back(new_node, make_pair(new_st, new_en));
						used[new_node].insert(make_pair(new_st, new_en));
					}
				}
			}
		}

		//output rules
		for (auto cp : H)
		{
			int node = cp.first;
			int st = cp.second.first;
			int en = cp.second.second;
			if (! contain_word[node].size())
				continue;
			vector<string> lhs, rhs;
			lhs.push_back(contain_word[node]);
			for (int j = st - 1; j < en; j ++)
				rhs.push_back(tokens[i][j]);
//			rhs.push_back(cur_string);
			rules.insert(make_pair(lhs, rhs));
		}
	}

	return vector<t_rule>(rules.begin(), rules.end());
}
