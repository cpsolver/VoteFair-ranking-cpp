rem note: can get MinGW libraries from https://sourceforge.net/projects/mingw-w64/

path=C:\Program Files (x86)\mingw-w64\i686-8.1.0-posix-dwarf-rt_v6-rev0\mingw32\bin\

set TestCasePath=C:\whatever_path\VoteFair-ranking-cpp\test_cases\

del votefair_ranking.exe

g++ votefair_ranking.cpp -o votefair_ranking

.\votefair_ranking < %TestCasePath%input_votefair_ranking_test_case_101.txt > %TestCasePath%output_votefair_ranking_test_case_101.txt
copy output_votefair_ranking_log.txt %TestCasePath%output_log_test_case_101.txt
.\votefair_ranking < %TestCasePath%input_votefair_ranking_test_case_102.txt > %TestCasePath%output_votefair_ranking_test_case_102.txt
copy output_votefair_ranking_log.txt %TestCasePath%output_log_test_case_102.txt
.\votefair_ranking < %TestCasePath%input_votefair_ranking_test_case_103.txt > %TestCasePath%output_votefair_ranking_test_case_103.txt
copy output_votefair_ranking_log.txt %TestCasePath%output_log_test_case_103.txt
.\votefair_ranking < %TestCasePath%input_votefair_ranking_test_case_104.txt > %TestCasePath%output_votefair_ranking_test_case_104.txt
copy output_votefair_ranking_log.txt %TestCasePath%output_log_test_case_104.txt
.\votefair_ranking < %TestCasePath%input_votefair_ranking_test_case_105.txt > %TestCasePath%output_votefair_ranking_test_case_105.txt
copy output_votefair_ranking_log.txt %TestCasePath%output_log_test_case_105.txt
.\votefair_ranking < %TestCasePath%input_votefair_ranking_test_case_106.txt > %TestCasePath%output_votefair_ranking_test_case_106.txt
copy output_votefair_ranking_log.txt %TestCasePath%output_log_test_case_106.txt
.\votefair_ranking < %TestCasePath%input_votefair_ranking_test_case_107.txt > %TestCasePath%output_votefair_ranking_test_case_107.txt
copy output_votefair_ranking_log.txt %TestCasePath%output_log_test_case_107.txt
.\votefair_ranking < %TestCasePath%input_votefair_ranking_test_case_108.txt > %TestCasePath%output_votefair_ranking_test_case_108.txt
copy output_votefair_ranking_log.txt %TestCasePath%output_log_test_case_108.txt
.\votefair_ranking < %TestCasePath%input_votefair_ranking_test_case_109.txt > %TestCasePath%output_votefair_ranking_test_case_109.txt
copy output_votefair_ranking_log.txt %TestCasePath%output_log_test_case_109.txt
.\votefair_ranking < %TestCasePath%input_votefair_ranking_test_case_110.txt > %TestCasePath%output_votefair_ranking_test_case_110.txt
copy output_votefair_ranking_log.txt %TestCasePath%output_log_test_case_110.txt
.\votefair_ranking < %TestCasePath%input_votefair_ranking_test_case_111.txt > %TestCasePath%output_votefair_ranking_test_case_111.txt
copy output_votefair_ranking_log.txt %TestCasePath%output_log_test_case_111.txt
.\votefair_ranking < %TestCasePath%input_votefair_ranking_test_case_112.txt > %TestCasePath%output_votefair_ranking_test_case_112.txt
copy output_votefair_ranking_log.txt %TestCasePath%output_log_test_case_112.txt
.\votefair_ranking < %TestCasePath%input_votefair_ranking_test_case_113.txt > %TestCasePath%output_votefair_ranking_test_case_113.txt
copy output_votefair_ranking_log.txt %TestCasePath%output_log_test_case_113.txt
.\votefair_ranking < %TestCasePath%input_votefair_ranking_test_case_114.txt > %TestCasePath%output_votefair_ranking_test_case_114.txt
copy output_votefair_ranking_log.txt %TestCasePath%output_log_test_case_114.txt
.\votefair_ranking < %TestCasePath%input_votefair_ranking_test_case_115.txt > %TestCasePath%output_votefair_ranking_test_case_115.txt
copy output_votefair_ranking_log.txt %TestCasePath%output_log_test_case_115.txt
.\votefair_ranking < %TestCasePath%input_votefair_ranking_test_case_116.txt > %TestCasePath%output_votefair_ranking_test_case_116.txt
copy output_votefair_ranking_log.txt %TestCasePath%output_log_test_case_116.txt
.\votefair_ranking < %TestCasePath%input_votefair_ranking_test_case_116.txt > %TestCasePath%output_votefair_ranking_test_case_117.txt
copy output_votefair_ranking_log.txt %TestCasePath%output_log_test_case_117.txt

type %TestCasePath%output_votefair_ranking_test_case_101.txt > %TestCasePath%combined_output_all_cases.txt
type %TestCasePath%output_votefair_ranking_test_case_102.txt >> %TestCasePath%combined_output_all_cases.txt
type %TestCasePath%output_votefair_ranking_test_case_103.txt >> %TestCasePath%combined_output_all_cases.txt
type %TestCasePath%output_votefair_ranking_test_case_104.txt >> %TestCasePath%combined_output_all_cases.txt
type %TestCasePath%output_votefair_ranking_test_case_105.txt >> %TestCasePath%combined_output_all_cases.txt
type %TestCasePath%output_votefair_ranking_test_case_106.txt >> %TestCasePath%combined_output_all_cases.txt
type %TestCasePath%output_votefair_ranking_test_case_107.txt >> %TestCasePath%combined_output_all_cases.txt
type %TestCasePath%output_votefair_ranking_test_case_108.txt >> %TestCasePath%combined_output_all_cases.txt
type %TestCasePath%output_votefair_ranking_test_case_109.txt >> %TestCasePath%combined_output_all_cases.txt
type %TestCasePath%output_votefair_ranking_test_case_110.txt >> %TestCasePath%combined_output_all_cases.txt
type %TestCasePath%output_votefair_ranking_test_case_111.txt >> %TestCasePath%combined_output_all_cases.txt
type %TestCasePath%output_votefair_ranking_test_case_112.txt >> %TestCasePath%combined_output_all_cases.txt
type %TestCasePath%output_votefair_ranking_test_case_113.txt >> %TestCasePath%combined_output_all_cases.txt
type %TestCasePath%output_votefair_ranking_test_case_114.txt >> %TestCasePath%combined_output_all_cases.txt
type %TestCasePath%output_votefair_ranking_test_case_115.txt >> %TestCasePath%combined_output_all_cases.txt
type %TestCasePath%output_votefair_ranking_test_case_116.txt >> %TestCasePath%combined_output_all_cases.txt
type %TestCasePath%output_votefair_ranking_test_case_117.txt >> %TestCasePath%combined_output_all_cases.txt

type %TestCasePath%answer_expected_for_case_101.txt > %TestCasePath%answers_expected_all_cases.txt
type %TestCasePath%answer_expected_for_case_102.txt >> %TestCasePath%answers_expected_all_cases.txt
type %TestCasePath%answer_expected_for_case_103.txt >> %TestCasePath%answers_expected_all_cases.txt
type %TestCasePath%answer_expected_for_case_104.txt >> %TestCasePath%answers_expected_all_cases.txt
type %TestCasePath%answer_expected_for_case_105.txt >> %TestCasePath%answers_expected_all_cases.txt
type %TestCasePath%answer_expected_for_case_106.txt >> %TestCasePath%answers_expected_all_cases.txt
type %TestCasePath%answer_expected_for_case_107.txt >> %TestCasePath%answers_expected_all_cases.txt
type %TestCasePath%answer_expected_for_case_108.txt >> %TestCasePath%answers_expected_all_cases.txt
type %TestCasePath%answer_expected_for_case_109.txt >> %TestCasePath%answers_expected_all_cases.txt
type %TestCasePath%answer_expected_for_case_110.txt >> %TestCasePath%answers_expected_all_cases.txt
type %TestCasePath%answer_expected_for_case_111.txt >> %TestCasePath%answers_expected_all_cases.txt
type %TestCasePath%answer_expected_for_case_112.txt >> %TestCasePath%answers_expected_all_cases.txt
type %TestCasePath%answer_expected_for_case_113.txt >> %TestCasePath%answers_expected_all_cases.txt
type %TestCasePath%answer_expected_for_case_114.txt >> %TestCasePath%answers_expected_all_cases.txt
type %TestCasePath%answer_expected_for_case_115.txt >> %TestCasePath%answers_expected_all_cases.txt
type %TestCasePath%answer_expected_for_case_116.txt >> %TestCasePath%answers_expected_all_cases.txt
type %TestCasePath%answer_expected_for_case_117.txt >> %TestCasePath%answers_expected_all_cases.txt

rem note: instead of kdiff3 you can use any of the tools listed at:
rem https://en.wikipedia.org/wiki/Comparison_of_file_comparison_tools

"C:/Program Files (x86)/KDiff3/kdiff3.exe" %TestCasePath%combined_output_all_cases.txt %TestCasePath%answers_expected_all_cases.txt

pause
