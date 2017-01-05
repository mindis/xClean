//
// Created by Wenbo Tao on 12/30/16.
//

#include "PolynomialJoiner.h"
#include "SigBuilder/DpSigBuilder.h"

PolynomialJoiner::PolynomialJoiner(vector<t_rule> r, vector<string> s, vector<int> w)
	: Joiner(r, s, w)
{
	//make global token ranked list
	umpsi g_token_map;
	for (int i = 0; i < n; i ++)
		for (string t : tokens[i])
			g_token_map[t] += weights[i];

	vector<pair<int, string>> sort_array;
	for (auto cp : g_token_map)
		sort_array.emplace_back(cp.second, cp.first);
	sort(sort_array.begin(), sort_array.end());

	for (int i = 0; i < (int) sort_array.size(); i ++)
	{
		global_list.push_back(sort_array[i].second);
		token_rankings[sort_array[i].second] = i;
	}

	//build signatures
	signatures.clear();
	for (int i = 0; i < n; i ++)
	{
		vector<t_rule> applicable_rules;
		for (int rule_id : applicable_rule_ids[i])
			applicable_rules.push_back(rules[rule_id]);

		SigBuilder *sigBuilder = new DpSigBuilder(tokens[i], applicable_rules, token_rankings, expansion_set[i], JAC_THRESHOLD);
		signatures.push_back(sigBuilder->genSignatures());

//		cout << i << " : " << signatures.back().size() << endl;
		delete sigBuilder;
	}

	cerr << "Signature built." << endl;
	//build inverted lists
	unordered_map<string, vector<int>> inv_list;
	for (int i = 0; i < n; i ++)
		for (string t : signatures[i])
			inv_list[t].push_back(i);

	int sum = 0;
	for (int i = 0; i < n; i ++)
	{
		unordered_set<int> candidates;
		for (string t : signatures[i])
			for (int v : inv_list[t])
				candidates.insert(v);
		sum += candidates.size();
	}
	cout << sum << endl;
}

vector<pair<string, string>> PolynomialJoiner::getJoinedStringPairs()
{
	return vector<pair<string, string>>();
}
