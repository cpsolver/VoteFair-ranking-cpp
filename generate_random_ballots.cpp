//  generate_random_ballots.cpp
//
//  This program generates random ballots for use with the
//  votefair_ranking.cpp code.
//
//
// -----------------------------------------------
//
//  COPYRIGHT & LICENSE
//
//  (c) Copyright 2020 by Richard Fobes at www.VoteFair.org.
//  You can redistribute and/or modify this VoteFairRanking software
//  under the MIT software license terms that appear
//  in the "license" file.
//
//  Conversion of this code into another programming language
//  is also covered by the above license terms.
//
//
// -----------------------------------------------
//
//  VERSION
//
//  Version 1.01
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

#include <cstring>
#include <string>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <random>
#include <chrono>


// -----------------------------------------------
//  Specify which test to run.

const int global_test_matches_with_votefair_ranking = 1 ;
const int global_test_irrelevant_alternatives = 2 ;
int global_test_type = global_test_irrelevant_alternatives ;
// int global_test_type = global_test_matches_with_votefair_ranking ;


// -----------------------------------------------
//  Specify the number of ballots and the number
//  of choices.

const int global_maximum_case_count = 200 ;
const int global_maximum_ballot_number = 17 ;
const int global_maximum_choice_number = 6 ;


// -----------------------------------------------
//  Define true and false constants (for easier
//  conversion between programming languages).

const int global_false = 0 ;
const int global_true = 1 ;


// -----------------------------------------------
//  Declare global variables.

int global_case_id = 0 ;
int global_count_of_ipe_cases_that_agree = 0 ;
int global_count_of_ipe_cases_that_disagree = 0 ;
int global_count_of_ipe_cases_tied = 0 ;
int global_count_of_ipe_cases_fail_irrel_alt = 0 ;
int global_count_of_irmpl_cases_that_agree = 0 ;
int global_count_of_irmpl_cases_that_disagree = 0 ;
int global_count_of_irmpl_cases_tied = 0 ;
int global_count_of_irmpl_cases_fail_irrel_alt = 0 ;
int global_count_of_irv_cases_that_agree = 0 ;
int global_count_of_irv_cases_that_disagree = 0 ;
int global_count_of_irv_cases_tied = 0 ;
int global_count_of_irv_cases_fail_irrel_alt = 0 ;
int global_count_of_star_cases_that_agree = 0 ;
int global_count_of_star_cases_that_disagree = 0 ;
int global_count_of_star_cases_tied = 0 ;
int global_count_of_star_cases_fail_irrel_alt = 0 ;
int global_count_of_ple_cases_that_agree = 0 ;
int global_count_of_ple_cases_that_disagree = 0 ;
int global_count_of_ple_cases_tied = 0 ;
int global_count_of_ple_cases_fail_irrel_alt = 0 ;
int global_count_of_votefair_group_failures_irrel_alt = 0 ;
int global_count_of_ipe_group_failures_irrel_alt = 0 ;
int global_count_of_irmpl_group_failures_irrel_alt = 0 ;
int global_count_of_star_group_failures_irrel_alt = 0 ;
int global_count_of_irv_group_failures_irrel_alt = 0 ;
int global_count_of_ple_group_failures_irrel_alt = 0 ;
int global_count_of_votefair_cases_fail_irrel_alt = 0 ;
int global_count_of_votefair_single_winner = 0 ;
int global_count_of_votefair_no_single_winner = 0 ;
int global_choice_count_case_specific = 0 ;
int global_choice_on_ballot_at_ranking_level[ 200 ][ 20 ] ;
int global_choice_omitted = 0 ;
int global_choice_winner_from_votefair_ranking_all_choices = 0 ;
int global_choice_winner_from_ipe_all_choices = 0 ;
int global_choice_winner_from_irmpl_all_choices = 0 ;
int global_choice_winner_from_star_all_choices = 0 ;
int global_choice_winner_from_irv_all_choices = 0 ;
int global_choice_winner_from_ple_all_choices = 0 ;
int global_count_of_irrel_alt_tests = 0 ;


// -----------------------------------------------
//  Specify codes that identify the meaning of the
//  next number in the coded output list.

std::string global_voteinfo_code_for_end_of_all_cases = "-2" ;
std::string global_voteinfo_code_for_case_number = "-3" ;
std::string global_voteinfo_code_for_question_number = "-4" ;
std::string global_voteinfo_code_for_number_of_choices = "-6" ;
std::string global_voteinfo_code_for_start_of_all_vote_info = "-7" ;
std::string global_voteinfo_code_for_end_of_all_vote_info = "-8" ;
std::string global_voteinfo_code_for_end_of_ballot = "-10" ;
std::string global_voteinfo_code_for_ballot_count = "-11" ;
std::string global_voteinfo_code_for_preference_level = "-12" ;

std::string global_voteinfo_code_for_request_instant_runoff_voting = "-50" ;
std::string global_voteinfo_code_for_request_instant_pairwise_elimination = "-51" ;
std::string global_voteinfo_code_for_request_irv_minus_pairwise_losers = "-52" ;
std::string global_voteinfo_code_for_request_star_voting = "-56" ;
std::string global_voteinfo_code_for_request_pairwise_loser_elimination = "-58" ;

const int global_voteinfo_code_for_choice = -13 ;
const int global_voteinfo_code_for_tie = -14 ;
const int global_voteinfo_code_for_start_of_votefair_popularity_ranking_sequence_results = -15 ;

const int global_voteinfo_code_for_winner_instant_runoff_voting = -53 ;
const int global_voteinfo_code_for_winner_instant_pairwise_elimination = -54 ;
const int global_voteinfo_code_for_winner_irv_minus_pairwise_losers = -55 ;
const int global_voteinfo_code_for_winner_star_voting = -57 ;
const int global_voteinfo_code_for_winner_pairwise_loser_elimination = -59 ;


// -----------------------------------------------
//  Declare an output file for writing log info.

std::ofstream log_out ;


// -----------------------------------------------
//  Declare an input file for reading calculated
//  results.

std::ifstream calc_results ;


// -----------------------------------------------
//  Declare the random number generator.

unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine generator ( seed ) ;


// -----------------------------------------------
//  Declare the needed arrays.

int global_choice_number_at_position[ 99 ] ;
int global_usage_count_for_choice_and_rank[ 99 ][ 99 ] ;


// -----------------------------------------------
//  Specify constants.

const int global_question_number = 1 ;
const int global_minimum_case_id = 100000 ;
const int global_maximum_case_id = 999999 ;


// -----------------------------------------------
//  convert_text_to_integer
//
//  This function is used instead of "std::to_string"
//  for compatibility with older C++ "string" libraries
//  that have a bug.  The bug is that the "to_string"
//  function is not recognized as being within the
//  "std" library, even though it is defined there.

int convert_text_to_integer( char * supplied_text )
{
    int equivalent_integer ;
    try
    {
        equivalent_integer = atoi( supplied_text ) ;
    }
    catch( ... )
    {
        equivalent_integer = -99999 ;
    }
    return equivalent_integer ;
}


// -----------------------------------------------
// -----------------------------------------------
//  generate_preferences
//
//  This function generates random preferences
//  for the ballots.

void generate_preferences( ) {


// -----------------------------------------------
//  Initialization.

    int ballot_number = 0 ;
    int choice_number = 0 ;
    int ranking_level = 0 ;
    int position_number = 0 ;
    int pointer_number = 0 ;
    int count_of_choices_not_yet_ranked = 0 ;

    for ( choice_number = 1 ; choice_number <= global_maximum_choice_number ; choice_number ++ )
    {
        for ( ranking_level = 1 ; ranking_level <= global_maximum_choice_number ; ranking_level ++ )
        {
            global_usage_count_for_choice_and_rank[ choice_number ][ ranking_level ] = 0 ;
        }
    }


// -----------------------------------------------
//  Begin a loop that handles one ballot.

    for ( ballot_number = 1 ; ballot_number <= global_maximum_ballot_number ; ballot_number ++ )
    {
//        log_out << "[" ;


// -----------------------------------------------
//  Put the choice numbers in a list so that
//  they can be chosen at random without repeating
//  any choice number.

        for ( choice_number = 1 ; choice_number <= global_maximum_choice_number ; choice_number ++ )
        {
            global_choice_number_at_position[ choice_number ] = choice_number ;
        }
        count_of_choices_not_yet_ranked = global_maximum_choice_number ;


// -----------------------------------------------
//  Begin a loop that handles one ranking level.

        for ( ranking_level = 1 ; ranking_level <= global_maximum_choice_number ; ranking_level ++ )
        {


// -----------------------------------------------
//  Randomly choose a choice number for this ranking
//  level.  Do not repeat any choice number already used
//  at a previous ranking level.
//  Also keep track of usage to verify randomness.

            std::uniform_int_distribution<int> distribution( 1 , count_of_choices_not_yet_ranked );
            position_number = distribution( generator );  
            choice_number = global_choice_number_at_position[ position_number ] ;
            global_choice_on_ballot_at_ranking_level[ ballot_number ][ ranking_level ] = choice_number ;
            for ( pointer_number = position_number ; pointer_number <= count_of_choices_not_yet_ranked - 1 ; pointer_number ++ )
            {
                global_choice_number_at_position[ pointer_number ] = global_choice_number_at_position[ pointer_number + 1 ] ;
            }
            count_of_choices_not_yet_ranked -- ;
            global_usage_count_for_choice_and_rank[ choice_number ][ ranking_level ] ++ ;
//            log_out << choice_number << " " ;


// -----------------------------------------------
//  Repeat the loop that handles one ranking level.

        }


// -----------------------------------------------
//  Repeat the loop that handles one ballot.

//        log_out << "]" << std::endl ;
    }


// -----------------------------------------------
//  Optionally verify that the preference info is
//  really random.

    if ( 1 == 1 )
    {
        std::fstream verifyfile;
        verifyfile.open ( "temp_verify_randomness.txt" , std::fstream::app ) ;
        for ( choice_number = 1 ; choice_number <= global_maximum_choice_number ; choice_number ++ )
        {
            for ( ranking_level = 1 ; ranking_level <= global_maximum_choice_number ; ranking_level ++ )
            {
                verifyfile << "choice " << choice_number << " rank " << ranking_level << " usage " << global_usage_count_for_choice_and_rank[ choice_number ][ ranking_level ] << std::endl ;
            }
        }
        verifyfile.close( ) ;
    }


// -----------------------------------------------
//  End of function generate_preferences.

    return ;

}


// -----------------------------------------------
// -----------------------------------------------
//     handle_calculated_results
//
//  Reads numbers and codes from the file
//  written by the calculation program.
//
// -----------------------------------------------
// -----------------------------------------------

void handle_calculated_results( )
{

    std::string input_line ;
    std::string input_text_word ;

    int current_result_code = 0 ;
    int previous_result_code = 0 ;
    int next_result_code = 0 ;
    int count_of_result_codes = 0 ;
    int choice_winner_from_votefair_ranking = 0 ;
    int choice_winner_from_ipe = 0 ;
    int choice_winner_from_irmpl = 0 ;
    int choice_winner_from_star = 0 ;
    int choice_winner_from_irv = 0 ;
    int choice_winner_from_ple = 0 ;
    int count_position_at_start_of_votefair_popularity_sequence = 0 ;
    int count_position_at_choice_number = 0 ;
    int choice_number_adjustment = 0 ;


// -----------------------------------------------
//  Begin loop to handle one line from the input file
//  that contains the calculated results.

    log_out << std::endl << "[" << global_choice_count_case_specific << " choices]" ;
    count_position_at_start_of_votefair_popularity_sequence = -10 ;
    count_position_at_choice_number = -10 ;
    for ( std::string input_line ; std::getline( calc_results , input_line ) ; )
    {
        std::size_t pointer_found = input_line.find_last_not_of( " \t\n\r" ) ;
        if ( pointer_found != std::string::npos )
        {
            input_line.erase( pointer_found + 1 ) ;
        } else
        {
            input_line.clear( ) ;
        }

//        log_out << "[input line: " << input_line << "]" << std::endl ;

        char input_line_c_version[ 2000 ] = "" ;
        std::size_t line_length = std::min( 2000 , (int) input_line.length() ) ;
        std::size_t line_length_copied = input_line.copy( input_line_c_version , line_length , 0 ) ;
        input_line_c_version[ line_length_copied ] = '\0' ;


// -----------------------------------------------
//  Begin loop to get first/next space-delimited
//  word of text from the input line.

        char * pointer_to_word ;
        // reminder: strtok modifies the string
        pointer_to_word = strtok( input_line_c_version , " ,." ) ;
        while ( pointer_to_word != NULL )
        {


// -----------------------------------------------
//  Attempt to convert the input word into an
//  integer.  If this conversion is not successful,
//  indicate an error and write this error to the
//  output files and then exit the program.

            try
            {
                current_result_code = convert_text_to_integer( pointer_to_word ) ;
//                log_out << "[" << current_result_code << "]  " ;
            }
            catch( ... )
            {
                log_out << "Error: invalid input word: " << pointer_to_word << std::endl ;
                log_out << "[Warning: Input line contains non-numeric characters (" << pointer_to_word << "), so this case (" << global_case_id << ") cannot be calculated]" << std::endl ;
                exit( EXIT_FAILURE ) ;
            }


// -----------------------------------------------
//  Handle the supplied vote-info number.

//            log_out << "[" << current_result_code << "]" ;
            if ( current_result_code == global_voteinfo_code_for_start_of_votefair_popularity_ranking_sequence_results )
            {
                count_position_at_start_of_votefair_popularity_sequence = count_of_result_codes ;
            } else if ( current_result_code == global_voteinfo_code_for_choice )
            {
                count_position_at_choice_number = count_of_result_codes ;
            } else if ( ( count_of_result_codes == count_position_at_start_of_votefair_popularity_sequence + 2 ) && ( count_of_result_codes == count_position_at_choice_number + 1 ) )
            {
                choice_winner_from_votefair_ranking = current_result_code ;
                choice_winner_from_ipe = 0 ;
                choice_winner_from_irv = 0 ;
                choice_winner_from_irmpl = 0 ;
                choice_winner_from_star = 0 ;
                log_out << "[vf " << choice_winner_from_votefair_ranking << "]" ;
            } else if ( ( current_result_code == global_voteinfo_code_for_tie ) && ( count_of_result_codes == count_position_at_start_of_votefair_popularity_sequence + 3 ) )
            {
                choice_winner_from_votefair_ranking = 0 ;
                log_out << "[vf_tie " << choice_winner_from_votefair_ranking << "]" ;
            } else if ( previous_result_code == global_voteinfo_code_for_winner_instant_pairwise_elimination )
            {
                choice_winner_from_ipe = current_result_code ;
                log_out << "[ipe " << choice_winner_from_ipe << "]" ;
            } else if ( previous_result_code == global_voteinfo_code_for_winner_irv_minus_pairwise_losers )
            {
                choice_winner_from_irmpl = current_result_code ;
                log_out << "[irmpl " << choice_winner_from_irmpl << "]" ;
            } else if ( previous_result_code == global_voteinfo_code_for_winner_instant_runoff_voting )
            {
                choice_winner_from_irv = current_result_code ;
                log_out << "[irv " << choice_winner_from_irv << "]" ;
            } else if ( previous_result_code == global_voteinfo_code_for_winner_star_voting )
            {
                choice_winner_from_star = current_result_code ;
                log_out << "[star " << choice_winner_from_star << "]" ;
            } else if ( previous_result_code == global_voteinfo_code_for_winner_pairwise_loser_elimination )
            {
                choice_winner_from_ple = current_result_code ;
                log_out << "[ple " << choice_winner_from_ple << "]" ;
            }


// -----------------------------------------------
//  Repeat the loop for the next word (within the line).

            count_of_result_codes ++ ;
            previous_result_code = current_result_code ;
            pointer_to_word = strtok( NULL, " ,." ) ;
        }


// -----------------------------------------------
//  Repeat the loop for the next line of data from
//  the input file.

    }


// -----------------------------------------------
//  For IPE, IVR, PL+IRV methods, count the number
//  of cases that agree with, and disagree with,
//  the winner according to VoteFair popularity
//  ranking.  Also count ties separately.
//  If the tie occurs in VoteFair popularity
//  ranking, the case is ignored.

    if ( global_choice_count_case_specific == global_maximum_choice_number )
    {
        if ( choice_winner_from_votefair_ranking > 0 )
        {
            global_count_of_votefair_single_winner ++ ;

            if ( choice_winner_from_ipe == choice_winner_from_votefair_ranking )
            {
                global_count_of_ipe_cases_that_agree ++ ;
            } else if ( choice_winner_from_ipe > 0 )
            {
                global_count_of_ipe_cases_that_disagree ++ ;
            } else
            {
                global_count_of_ipe_cases_tied ++ ;
            }

            if ( choice_winner_from_irmpl == choice_winner_from_votefair_ranking )
            {
                global_count_of_irmpl_cases_that_agree ++ ;
            } else if ( choice_winner_from_irmpl > 0 )
            {
                global_count_of_irmpl_cases_that_disagree ++ ;
            } else
            {
                global_count_of_irmpl_cases_tied ++ ;
            }

            if ( choice_winner_from_irv == choice_winner_from_votefair_ranking )
            {
                global_count_of_irv_cases_that_agree ++ ;
            } else if ( choice_winner_from_irv > 0 )
            {
                global_count_of_irv_cases_that_disagree ++ ;
            } else
            {
                global_count_of_irv_cases_tied ++ ;
            }

            if ( choice_winner_from_star == choice_winner_from_votefair_ranking )
            {
                global_count_of_star_cases_that_agree ++ ;
            } else if ( choice_winner_from_star > 0 )
            {
                global_count_of_star_cases_that_disagree ++ ;
            } else
            {
                global_count_of_star_cases_tied ++ ;
            }

            if ( choice_winner_from_ple == choice_winner_from_votefair_ranking )
            {
                global_count_of_ple_cases_that_agree ++ ;
            } else if ( choice_winner_from_ple > 0 )
            {
                global_count_of_ple_cases_that_disagree ++ ;
            } else
            {
                global_count_of_ple_cases_tied ++ ;
            }

        } else
        {
            global_count_of_votefair_no_single_winner ++ ;
        }
    }


// -----------------------------------------------
//  If this case omits a non-winning choice,
//  determine whether the winner is different from
//  the case with all the choices.  If so, count
//  it as a failure for the counting method.
//  This tests for cases in which the presence of
//  a non-winning choice (candidate) changes which
//  choice wins.

    if ( global_choice_count_case_specific == global_maximum_choice_number )
    {
        global_choice_winner_from_votefair_ranking_all_choices = choice_winner_from_votefair_ranking ;
        global_choice_winner_from_ipe_all_choices = choice_winner_from_ipe ;
        global_choice_winner_from_irmpl_all_choices = choice_winner_from_irmpl ;
        global_choice_winner_from_star_all_choices = choice_winner_from_star ;
        global_choice_winner_from_irv_all_choices = choice_winner_from_irv ;
        global_choice_winner_from_ple_all_choices = choice_winner_from_ple ;

    } else
    {

        if ( ( choice_winner_from_votefair_ranking > 0 ) && ( global_choice_winner_from_votefair_ranking_all_choices > 0 ) && ( global_choice_omitted != global_choice_winner_from_votefair_ranking_all_choices ) )
        {
            choice_number_adjustment = 0 ;
            if ( global_choice_winner_from_votefair_ranking_all_choices > global_choice_omitted )
            {
                choice_number_adjustment = 1 ;
            }
            if ( choice_winner_from_votefair_ranking + choice_number_adjustment != global_choice_winner_from_votefair_ranking_all_choices )
            {
                global_count_of_votefair_group_failures_irrel_alt ++ ;
                log_out << "[vf_irrel]" ;
            }
        }

        if ( ( choice_winner_from_ipe > 0 ) && ( global_choice_winner_from_ipe_all_choices > 0 ) && ( global_choice_omitted != global_choice_winner_from_ipe_all_choices ) )
        {
            choice_number_adjustment = 0 ;
            if ( global_choice_winner_from_ipe_all_choices > global_choice_omitted )
            {
                choice_number_adjustment = 1 ;
            }
            if ( choice_winner_from_ipe + choice_number_adjustment != global_choice_winner_from_ipe_all_choices )
            {
                global_count_of_ipe_group_failures_irrel_alt ++ ;
                log_out << "[ipe_irrel]" ;
            }
        }

        if ( ( choice_winner_from_irmpl > 0 ) && ( global_choice_winner_from_irmpl_all_choices > 0 ) && ( global_choice_omitted != global_choice_winner_from_irmpl_all_choices ) )
        {
            choice_number_adjustment = 0 ;
            if ( global_choice_winner_from_irmpl_all_choices > global_choice_omitted )
            {
                choice_number_adjustment = 1 ;
            }
            if ( choice_winner_from_irmpl + choice_number_adjustment != global_choice_winner_from_irmpl_all_choices )
            {
                global_count_of_irmpl_group_failures_irrel_alt ++ ;
                log_out << "[irmpl_irrel]" ;
            }
        }

        if ( ( choice_winner_from_star > 0 ) && ( global_choice_winner_from_star_all_choices > 0 ) && ( global_choice_omitted != global_choice_winner_from_star_all_choices ) )
        {
            choice_number_adjustment = 0 ;
            if ( global_choice_winner_from_star_all_choices > global_choice_omitted )
            {
                choice_number_adjustment = 1 ;
            }
            if ( choice_winner_from_star + choice_number_adjustment != global_choice_winner_from_star_all_choices )
            {
                global_count_of_star_group_failures_irrel_alt ++ ;
                log_out << "[star_irrel]" ;
            }
        }

        if ( ( choice_winner_from_irv > 0 ) && ( global_choice_winner_from_irv_all_choices > 0 ) && ( global_choice_omitted != global_choice_winner_from_irv_all_choices ) )
        {
            choice_number_adjustment = 0 ;
            if ( global_choice_winner_from_irv_all_choices > global_choice_omitted )
            {
                choice_number_adjustment = 1 ;
            }
            if ( choice_winner_from_irv + choice_number_adjustment != global_choice_winner_from_irv_all_choices )
            {
                global_count_of_irv_group_failures_irrel_alt ++ ;
                log_out << "[irv_irrel]" ;
            }
        }

        if ( ( choice_winner_from_ple > 0 ) && ( global_choice_winner_from_ple_all_choices > 0 ) && ( global_choice_omitted != global_choice_winner_from_ple_all_choices ) )
        {
            choice_number_adjustment = 0 ;
            if ( global_choice_winner_from_ple_all_choices > global_choice_omitted )
            {
                choice_number_adjustment = 1 ;
            }
            if ( choice_winner_from_ple + choice_number_adjustment != global_choice_winner_from_ple_all_choices )
            {
                global_count_of_ple_group_failures_irrel_alt ++ ;
                log_out << "[ple_irrel]" ;
            }
        }

        if ( global_choice_omitted == global_maximum_choice_number )
        {
            if ( global_count_of_votefair_group_failures_irrel_alt > 0 )
            {
                global_count_of_votefair_cases_fail_irrel_alt ++ ;
            }
            if ( global_count_of_ipe_group_failures_irrel_alt > 0 )
            {
                global_count_of_ipe_cases_fail_irrel_alt ++ ;
            }
            if ( global_count_of_irmpl_group_failures_irrel_alt > 0 )
            {
                global_count_of_irmpl_cases_fail_irrel_alt ++ ;
            }
            if ( global_count_of_star_group_failures_irrel_alt > 0 )
            {
                global_count_of_star_cases_fail_irrel_alt ++ ;
            }
            if ( global_count_of_irv_group_failures_irrel_alt > 0 )
            {
                global_count_of_irv_cases_fail_irrel_alt ++ ;
            }
            if ( global_count_of_ple_group_failures_irrel_alt > 0 )
            {
                global_count_of_ple_cases_fail_irrel_alt ++ ;
            }
        }
    }


// -----------------------------------------------
//  End of function handle_calculated_results

    return ;

}


// -----------------------------------------------
// -----------------------------------------------
//  Execution starts here.

int main( ) {


// -----------------------------------------------
//  Declare integer variables.

    int case_count = 0 ;
    int ballot_number = 0 ;
    int choice_number = 0 ;
    int ranking_level = 0 ;
    int position_number = 0 ;
    int pointer_number = 0 ;
    int count_of_choices_not_yet_ranked = 0 ;
    int integer_from_system_call = 0 ;
    int calculated_result_agree = 0 ;
    int calculated_result_disagree = 0 ;
    int calculated_result_tied = 0 ;
    int calculated_result_failures = 0 ;
    int count_of_cases_agree_plus_disagree = 0 ;


// -----------------------------------------------
//  Open the output log file.

    log_out.open ( "temp_log_from_generate_random_ballots.txt" , std::ios::out ) ;


// -----------------------------------------------
//  Begin a loop that handles one case.

    global_case_id = global_minimum_case_id ;
    global_choice_omitted = global_maximum_choice_number + 99 ;
    for ( case_count = 1 ; case_count <= global_maximum_case_count ; case_count ++ )
    {
        std::cout << "." ;


// -----------------------------------------------
//  For use when testing irrelevant alternatives,
//  update the global_choice_omitted counter.
//  It equals zero when all choices are used, and
//  otherwise it equals the choice to omit.

        if ( global_choice_omitted > global_maximum_choice_number )
        {
            global_choice_omitted = 0 ;
            global_count_of_votefair_group_failures_irrel_alt = 0 ;
            global_count_of_irrel_alt_tests ++ ;
        }


// -----------------------------------------------
//  If the test for irrelevant alternatives is
//  being done and one of the choices is being
//  omitted, specify one less than the usual
//  number of choices.

        global_choice_count_case_specific = global_maximum_choice_number ;
        if ( ( global_test_type == global_test_irrelevant_alternatives ) && ( global_choice_omitted > 0 ) )
        {
            global_choice_count_case_specific = global_maximum_choice_number - 1 ;
        }


// -----------------------------------------------
//  When needed, randomly generate new ballot
//  rankings.

        if ( global_choice_count_case_specific == global_maximum_choice_number )
        {
            generate_preferences( ) ;
        }


// -----------------------------------------------
//  Write the beginning of a new output file.

        std::fstream outfile ;
        outfile.open ( "temp_generated_random_ballots.txt" , std::fstream::out ) ;
        outfile << global_voteinfo_code_for_start_of_all_vote_info << std::endl ;
        outfile << global_voteinfo_code_for_case_number << " " << global_case_id << std::endl ;
        outfile << global_voteinfo_code_for_request_instant_pairwise_elimination << std::endl ;
        outfile << global_voteinfo_code_for_request_instant_runoff_voting << std::endl ;
        outfile << global_voteinfo_code_for_request_irv_minus_pairwise_losers << std::endl ;
        outfile << global_voteinfo_code_for_request_star_voting << std::endl ;
        outfile << global_voteinfo_code_for_request_pairwise_loser_elimination << std::endl ;
        outfile << global_voteinfo_code_for_question_number << " " << global_question_number << std::endl ;
        outfile << global_voteinfo_code_for_number_of_choices << " " << global_choice_count_case_specific << std::endl ;


// -----------------------------------------------
//  Begin a loop that handles one ballot.

        for ( ballot_number = 1 ; ballot_number <= global_maximum_ballot_number ; ballot_number ++ )
        {
            outfile << global_voteinfo_code_for_ballot_count << " 1" << std::endl ;
            outfile << global_voteinfo_code_for_question_number << " " << global_question_number << std::endl ;


// -----------------------------------------------
//  Begin a loop that handles one ranking level.

            for ( ranking_level = 1 ; ranking_level <= global_choice_count_case_specific ; ranking_level ++ )
            {


// -----------------------------------------------
//  Write the choice number that is at the
//  next-lower preference level.  But not if it
//  is an omitted choice.  Also, adjust the choice
//  number if needed.

                choice_number = global_choice_on_ballot_at_ranking_level[ ballot_number ][ ranking_level ] ;
                if ( global_choice_count_case_specific == global_maximum_choice_number )
                {
                    outfile << choice_number << std::endl ;
                } else
                {
                    if ( choice_number < global_choice_omitted )
                    {
                        outfile << choice_number << std::endl ;
                    } else if ( choice_number > global_choice_omitted )
                    {
                        choice_number -- ;
                        outfile << choice_number << std::endl ;
                    }
                }


// -----------------------------------------------
//  Repeat the loop that handles one preference ranking.

            }


// -----------------------------------------------
//  Indicate the end of this ballot.

            outfile << global_voteinfo_code_for_end_of_ballot << std::endl ;


// -----------------------------------------------
//  Repeat the loop that handles one ballot.

        }


// -----------------------------------------------
//  Write the codes at the end of the output file.

        outfile << global_voteinfo_code_for_end_of_all_vote_info << std::endl ;
        outfile << global_voteinfo_code_for_end_of_all_cases << std::endl ;


// -----------------------------------------------
//  Close the output file that contains the ballots.

        outfile.close( ) ;


// -----------------------------------------------
//  Run the VoteFair-ranking-cpp program.
//
//  documentation:  https://cplusplus.com/reference/cstdlib/system/

        integer_from_system_call = system( ".\\votefair_ranking < temp_generated_random_ballots.txt > temp_votefair_ranking_output.txt" ) ;
        outfile << integer_from_system_call << std::endl ;


// -----------------------------------------------
//  Join the log files to allow viewing details
//  of specific cases of interest.

        integer_from_system_call = system( "type output_votefair_ranking_log.txt >> temp_joined_output_votefair_ranking_log.txt" ) ;


// -----------------------------------------------
//  Open the calculated-results file for reading,
//  then read the calculated results, and count the
//  number of correct, incorrect, and tied
//  calculated results.

        calc_results.open ( "temp_votefair_ranking_output.txt" , std::ios::in ) ;
        handle_calculated_results( ) ;
        calc_results.close ( ) ;


// -----------------------------------------------
//  Update the case number.  Also update the
//  choice omitted number in case it is being used.

        global_case_id ++ ;  
        global_choice_omitted ++ ;


// -----------------------------------------------
//  Repeat the loop that handles one case.

    }


// -----------------------------------------------
//  Write the results.

    log_out << std::endl << std::endl << std::endl ;
    log_out << "[number of cases: " << global_maximum_case_count << "]" << std::endl ;
    log_out << "[number of ballots: " << global_maximum_ballot_number << "]" << std::endl ;
    log_out << "[number of choices: " << global_maximum_choice_number << "]" << std::endl ;

    if ( global_test_type == global_test_matches_with_votefair_ranking )
    {

        count_of_cases_agree_plus_disagree = global_count_of_ipe_cases_that_agree + global_count_of_ipe_cases_that_disagree ;
        calculated_result_agree = int( ( 1000 * global_count_of_ipe_cases_that_agree ) / count_of_cases_agree_plus_disagree ) ;
        calculated_result_disagree = int( ( 1000 * global_count_of_ipe_cases_that_disagree ) / count_of_cases_agree_plus_disagree ) ;
        calculated_result_tied = global_count_of_ipe_cases_tied ;
        log_out << "[IPE agree/disagree: " << calculated_result_agree << "  " << calculated_result_disagree << "  (" << calculated_result_tied << " ties)]" << std::endl ;

        count_of_cases_agree_plus_disagree = global_count_of_irmpl_cases_that_agree + global_count_of_irmpl_cases_that_disagree ;
        calculated_result_agree = int( ( 1000 * global_count_of_irmpl_cases_that_agree ) / count_of_cases_agree_plus_disagree ) ;
        calculated_result_disagree = int( ( 1000 * global_count_of_irmpl_cases_that_disagree ) / count_of_cases_agree_plus_disagree ) ;
        calculated_result_tied = global_count_of_irmpl_cases_tied ;
        log_out << "[IRMPL agree/disagree: " << calculated_result_agree << "  " << calculated_result_disagree << "  (" << calculated_result_tied << " ties)]" << std::endl ;

        count_of_cases_agree_plus_disagree = global_count_of_star_cases_that_agree + global_count_of_star_cases_that_disagree ;
        calculated_result_agree = int( ( 1000 * global_count_of_star_cases_that_agree ) / count_of_cases_agree_plus_disagree ) ;
        calculated_result_disagree = int( ( 1000 * global_count_of_star_cases_that_disagree ) / count_of_cases_agree_plus_disagree ) ;
        calculated_result_tied = global_count_of_star_cases_tied ;
        log_out << "[STAR agree/disagree: " << calculated_result_agree << "  " << calculated_result_disagree << "  (" << calculated_result_tied << " ties)]" << std::endl ;

        count_of_cases_agree_plus_disagree = global_count_of_irv_cases_that_agree + global_count_of_irv_cases_that_disagree ;
        calculated_result_agree = int( ( 1000 * global_count_of_irv_cases_that_agree ) / count_of_cases_agree_plus_disagree ) ;
        calculated_result_disagree = int( ( 1000 * global_count_of_irv_cases_that_disagree ) / count_of_cases_agree_plus_disagree ) ;
        calculated_result_tied = global_count_of_irv_cases_tied ;
        log_out << "[IRV agree/disagree: " << calculated_result_agree << "  " << calculated_result_disagree << "  (" << calculated_result_tied << " ties)]" << std::endl ;

        count_of_cases_agree_plus_disagree = global_count_of_ple_cases_that_agree + global_count_of_ple_cases_that_disagree ;
        calculated_result_agree = int( ( 1000 * global_count_of_ple_cases_that_agree ) / count_of_cases_agree_plus_disagree ) ;
        calculated_result_disagree = int( ( 1000 * global_count_of_ple_cases_that_disagree ) / count_of_cases_agree_plus_disagree ) ;
        calculated_result_tied = global_count_of_ple_cases_tied ;
        log_out << "[PLE agree/disagree: " << calculated_result_agree << "  " << calculated_result_disagree << "  (" << calculated_result_tied << " ties)]" << std::endl ;
    }

    if ( global_test_type == global_test_irrelevant_alternatives )
    {

        calculated_result_failures = int( ( 1000 *  global_count_of_votefair_cases_fail_irrel_alt ) / global_count_of_irrel_alt_tests ) ;
        log_out << "[vf irrel alt failures per k: " << calculated_result_failures << "]" << std::endl ;

        calculated_result_failures = int( ( 1000 *  global_count_of_ipe_cases_fail_irrel_alt ) / global_count_of_irrel_alt_tests ) ;
        log_out << "[ipe irrel alt failures per k: " << calculated_result_failures << "]" << std::endl ;

        calculated_result_failures = int( ( 1000 *  global_count_of_irmpl_cases_fail_irrel_alt ) / global_count_of_irrel_alt_tests ) ;
        log_out << "[irmpl irrel alt failures per k: " << calculated_result_failures << "]" << std::endl ;

        calculated_result_failures = int( ( 1000 *  global_count_of_star_cases_fail_irrel_alt ) / global_count_of_irrel_alt_tests ) ;
        log_out << "[star irrel alt failures per k: " << calculated_result_failures << "]" << std::endl ;

        calculated_result_failures = int( ( 1000 *  global_count_of_ple_cases_fail_irrel_alt ) / global_count_of_irrel_alt_tests ) ;
        log_out << "[ple irrel alt failures per k: " << calculated_result_failures << "]" << std::endl ;

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
