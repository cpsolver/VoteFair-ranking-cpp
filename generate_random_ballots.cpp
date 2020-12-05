//  generate_random_ballots.cpp
//
//
// -----------------------------------------------
//
//  COPYRIGHT & LICENSE
//
//  (c) Copyright 2020 by Richard Fobes at www.VoteFair.org.
//  You can redistribute and/or modify this VoteFairRanking software
//  under the MIT software license terms that appear below.
//  (Also a copy of the license is included in the "license" file.)
//
//  Conversion of this code into another programming language
//  is also covered by the above license terms.
//
//
// -----------------------------------------------
//
//  VERSION
//
//  Version 0.01
//
//
// -----------------------------------------------
//
//  USAGE
//
//  This program generates random ballots for use with the
//  votefair_ranking.cpp code.
//
//  The following sample code executes this software under a typical
//  Windows environment with the g++ compiler and the mingw32 library
//  already installed.
//
//      path=C:\Program Files (x86)\mingw-w64\i686-8.1.0-posix-dwarf-rt_v6-rev0\mingw32\bin\
//
//      g++ generate_random_ballots.cpp -o generate_random_ballots
//
//      .\generate_random_ballots > output_generate_random_ballots.txt
//
//
// -----------------------------------------------
//
//  ABOUT
//
//  This software generates random ballots for the
//  votefair_ranking.cpp code.
//
//
// -----------------------------------------------
//
//  LICENSE
//
//  This generate_random_ballots software is licensed
//  under the MIT License terms.
//
//
// -----------------------------------------------


// -----------------------------------------------
//  Begin code.
// -----------------------------------------------


// -----------------------------------------------
//  Specify libraries needed.

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdio>
#include <random>
#include <chrono>


// -----------------------------------------------
//  convert_integer_to_text
//
//  This function is used instead of "std::to_string"
//  for compatibility with older C++ "string" libraries
//  that have a bug.  The bug is that the "to_string"
//  function is not recognized as being within the
//  "std" library, even though it is defined there.

std::string convert_integer_to_text( int supplied_integer )
{
    int unused_string_length ;
    char c_format_string[ 50 ] ;
    try
    {
        unused_string_length = sprintf( c_format_string , "%1d" , supplied_integer ) ;
        return ( std::string ) c_format_string ;
    }
    catch( ... )
    {
    	return "NAN" ;
    }
}


// -----------------------------------------------
//  convert_text_to_integer
//
//  To read why this function is here, see the comment
//  above for function: convert_integer_to_text

int convert_text_to_integer( char * supplied_text )
{
    int equivalent_integer ;
    try
    {
        equivalent_integer = atoi( supplied_text ) ;
    }
    catch( ... )
    {
        equivalent_integer = -999 ;
    }
    return equivalent_integer ;
}


// -----------------------------------------------
// -----------------------------------------------
//  Execution starts here.

int main() {


// -----------------------------------------------
//  Declare the random number generator.

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator (seed);


// -----------------------------------------------
//  Declare the needed arrays.

    int ranking_level_at_position[ 99 ] ;
    int usage_count_for_choice_and_rank[ 99 ][ 99 ] ;


// -----------------------------------------------
//  Specify the number of ballots and the number
//  of choices.

    const int maximum_ballot_number = 17 ;
    const int maximum_choice_number = 4 ;


// -----------------------------------------------
//  Specify constants.

    const int global_yes = 1 ;
    const int global_no = 0 ;
    const int question_number = 1 ;
    const int maximum_case_number = 9999999 ;


// -----------------------------------------------
//  Initialization.

    int case_number = 0 ;
    int ballot_number = 0 ;
    int choice_number = 0 ;
    int ranking_level = 0 ;
    int position_number = 0 ;
    int pointer_number = 0 ;
    int count_of_choices_not_yet_ranked = 0 ;

    for ( choice_number = 1 ; choice_number <= maximum_choice_number ; choice_number ++ )
    {
        for ( ranking_level = 1 ; ranking_level <= maximum_choice_number ; ranking_level ++ )
        {
            usage_count_for_choice_and_rank[ choice_number ][ ranking_level ] = 0 ;
        }
    }


// -----------------------------------------------
//  Specify codes that identify the meaning of the
//  next number in the coded output list.

	std::string voteinfo_code_for_start_of_all_cases = "-1" ;
	std::string voteinfo_code_for_end_of_all_cases = "-2" ;
	std::string voteinfo_code_for_case_number = "-3" ;
	std::string voteinfo_code_for_question_number = "-4" ;
	std::string voteinfo_code_for_total_ballot_count = "-5" ;
	std::string voteinfo_code_for_number_of_choices = "-6" ;
	std::string voteinfo_code_for_start_of_all_vote_info = "-7" ;
	std::string voteinfo_code_for_end_of_all_vote_info = "-8" ;
	std::string voteinfo_code_for_start_of_ballot = "-9" ;
	std::string voteinfo_code_for_end_of_ballot = "-10" ;
	std::string voteinfo_code_for_preference_level = "-12" ;
	std::string voteinfo_code_for_choice = "-13" ;


// -----------------------------------------------
//  Randomly generate a case number.

    std::uniform_int_distribution<int> distribution( 1 , maximum_case_number );
    case_number = distribution( generator );  


// -----------------------------------------------
//  Write the beginning of the output file.

    std::cout << voteinfo_code_for_start_of_all_cases << std::endl ;
    std::cout << voteinfo_code_for_start_of_all_vote_info << std::endl ;
    std::cout << voteinfo_code_for_case_number << " " << case_number << std::endl ;
    std::cout << voteinfo_code_for_question_number << " " << question_number << std::endl ;
    std::cout << voteinfo_code_for_total_ballot_count << " " << maximum_ballot_number << std::endl ;


// -----------------------------------------------
//  Begin a loop that handles one ballot.

    for ( ballot_number = 1 ; ballot_number <= maximum_ballot_number ; ballot_number ++ )
    {
        std::cout << voteinfo_code_for_start_of_ballot << " " << std::endl ;


// -----------------------------------------------
//  Put the ranking levels in a list so that
//  they can be chosen at random without repeating
//  any ranking level.

        for ( choice_number = 1 ; choice_number <= maximum_choice_number ; choice_number ++ )
        {
        	ranking_level_at_position[ choice_number ] = choice_number ;
        }
        count_of_choices_not_yet_ranked = maximum_choice_number ;


// -----------------------------------------------
//  Begin a loop that handles one choice.
//  In elections the choice number refers to
//  a candidate number, but other things
//  besides candidates can be ranked.

        for ( choice_number = 1 ; choice_number <= maximum_choice_number ; choice_number ++ )
        {


// -----------------------------------------------
//  Randomly generate a ranking level for this choice,
//  but do not repeat any ranking level already used
//  for another choice.
//  Also keep track of usage to verify randomness.

            std::uniform_int_distribution<int> distribution( 1 , count_of_choices_not_yet_ranked );
            position_number = distribution( generator );  
        	ranking_level = ranking_level_at_position[ position_number ] ;
            for ( pointer_number = position_number ; pointer_number <= count_of_choices_not_yet_ranked - 1 ; pointer_number ++ )
            {
                ranking_level_at_position[ pointer_number ] = ranking_level_at_position[ pointer_number + 1 ] ;
            }
            count_of_choices_not_yet_ranked -- ;
            usage_count_for_choice_and_rank[ choice_number ][ ranking_level ] ++ ;


// -----------------------------------------------
//  Write the info for one ranking.

            std::cout << voteinfo_code_for_preference_level << " " << convert_integer_to_text( ranking_level ) << " " << voteinfo_code_for_choice << " " << convert_integer_to_text( choice_number ) << std::endl ;


// -----------------------------------------------
//  Repeat the loop that handles one preference ranking.

        }


// -----------------------------------------------
//  Repeat the loop that handles one ballot.

            std::cout << voteinfo_code_for_end_of_ballot << std::endl ;
    }


// -----------------------------------------------
//  Write the codes at the end of the output file.

    std::cout << voteinfo_code_for_end_of_all_vote_info << std::endl ;
    std::cout << voteinfo_code_for_end_of_all_cases << std::endl ;


// -----------------------------------------------
//  Optionally verify that the preference info is
//  really random.

    if ( global_yes == global_no )
    {
	    for ( choice_number = 1 ; choice_number <= maximum_choice_number ; choice_number ++ )
	    {
	        for ( ranking_level = 1 ; ranking_level <= maximum_choice_number ; ranking_level ++ )
	        {
	            std::cout << "choice " << choice_number << " rank " << ranking_level << " " << usage_count_for_choice_and_rank[ choice_number ][ ranking_level ] << std::endl ;
	        }
	    }
    }


// -----------------------------------------------
//  End of "main" code.

}


// -----------------------------------------------
// -----------------------------------------------
//
//  End of all code.
//
// -----------------------------------------------
// -----------------------------------------------


// -----------------------------------------------
//
//  AUTHOR
//
//  Richard Fobes, www.VoteFair.org
//
//
// -----------------------------------------------
//
//  BUGS
//
//  Please report any bugs or feature requests on GitHub,
//  at the CPSolver account, in the VoteFairRanking project
//  area.  Thank you!
//
//
// -----------------------------------------------
//
//  SUPPORT
//
//  You can find documentation for this code on GitHub,
//  in the CPSolver account, in the VoteFair Ranking project
//  areas.
//
//  You can find details about VoteFair Ranking at: www.VoteFair.org
//
//
// -----------------------------------------------
//
//  COPYRIGHT & LICENSE
//
//  (c) Copyright 2020 by Richard Fobes at www.VoteFair.org.
//  You can redistribute and/or modify this VoteFairRanking software
//  under the MIT software license terms as explained in the
//  LICENSE file.)
//
//
// -----------------------------------------------
//
//  End of generate_random_ballots.cpp