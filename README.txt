//  VoteFairRanking.cpp or votefair_ranking.cpp
//
//  This application calculates VoteFair Ranking results.
//  Specifically it calculates:
//
//  * VoteFair popularity ranking
//
//  * VoteFair representation ranking  
//
//  * VoteFair party ranking  
//
//  For detailed descriptions of VoteFair Ranking, see
//  www.VoteFair.org or the book "Ending The Hidden Unfairness In
//  U.S. Elections" by Richard Fobes.
//
//
// -----------------------------------------------
//
//  COPYRIGHT & LICENSE
//
//  (c) Copyright 1991 through 2019 by Richard Fobes at www.VoteFair.org.
//  You can redistribute and/or modify this VoteFairRanking software
//  under the MIT software license terms that appear in the source-code file.
//  Also a copy of the license is included in the LICENSE file.
//  Conversion of this code into another programming language
//  is also covered by these license terms.
//
//
// -----------------------------------------------
//
//  ABOUT
//
//  This software calculates VoteFair Ranking results.  The portions
//  of VoteFair Ranking implemented here are:
//
//  * VoteFair popularity ranking.  This voting method calculates the
//  full popularity ranking of all candidates (or choices in the case
//  of a survey) from most popular and second-most popular down to
//  least popular.  It uses the preference information collected on
//  1-2-3 ballots (or any equivalent way of expressing "ranked"
//  preferences).  When a single position is being filled, the most
//  popular candidate is declared the winner.  This calculation method
//  is mathematically equivalent to the Condorcet-Kemeny election
//  method.
//
//  * VoteFair representation ranking.  This voting method is used to
//  elect a second candidate who represents the voters who are not
//  well-represented by the most-popular candidate, or to fill
//  multiple board-of-director positions, or to choose a second
//  simultaneous activity in addition to the most popular activity.
//  This method reduces the influence of the voters who are already
//  well-represented by the most popular candidate (or choice), and
//  it does so in a way that protects against strategic voting.  If
//  instead the second-most popular candidate as identified by
//  VoteFair popularity ranking were chosen, the same voters who
//  prefer the first winner also can determine the second winner, and
//  this can leave large numbers of other voters unrepresented.
//  Additional levels of representation ranking can be used to fill
//  additional seats, although VoteFair partial-proportional ranking
//  should be used instead if "proportional representation" of
//  political parties is needed, especially for the purpose of
//  defeating attempts to gerrymander district boundaries.
//  This ranking provides "proportional representation."  It ignores
//  political-party associations, yet when used in a governmental
//  election the winners are typically from different political
//  parties (unless just one party offers great candidates and the
//  other parties offer bad candidates).
//
//  * VoteFair party ranking.  This voting method ranks political
//  parties according to a different kind of "popularity".  The
//  results can be used in high-stakes elections to limit the number
//  of candidates allowed by each party.  In such cases the two or
//  three political parties that are ranked highest can be limited to
//  offering just two candidates from each party, and lower-ranked
//  parties can be allowed to offer one candidate each, and any
//  additional parties can be prohibited from offering any candidate
//  (because those parties are too unpopular and too
//  unrepresentative).  Such limits have not been needed in the past
//  because the fear of vote splitting has limited each political
//  party to offering just one candidate in each contest.
//
//  Note that VoteFair partial-proportional ranking is not calculated
//  by this software because it requires knowing the results of many
//  elections, and because those results can be calculated using a
//  simple spreadsheet.
//
//  In addition to being useful for elections, VoteFair Ranking also
//  is useful for calculating results for surveys and polls, ranking
//  the popularity of songs and movies, and much more.
//
//  In mathematical terms, VoteFair Ranking is useful for doing
//  "combinatorial optimization" and may be useful for solving the
//  "linear ordering problem".  See Wikipedia for details about these
//  terms.
//
//  Also in mathematical terms, the VoteFair popularity ranking
//  calculations are mathematically equivalent to the
//  Condorcet-Kemeny vote-counting method, which is also described in
//  Wikipedia.
//
//  Additional details about the calculations appear within comments
//  in the source-code file.
//
// -----------------------------------------------
