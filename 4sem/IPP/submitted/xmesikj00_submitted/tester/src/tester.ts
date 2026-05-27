#!/usr/bin/env node
/**
 * An integration testing script for the SOL26 interpreter.
 *
 * IPP: You can implement the entire tool in this file if you wish, but it is recommended to split
 *      the code into multiple files and modules as you see fit.
 *
 *      Below, you have some code to get you started with the CLI argument parsing and logging setup,
 *      but you are **free to modify it** in whatever way you like.
 *
 * Author: Ondrej Ondryas <iondryas@fit.vut.cz>
 */

import {
  existsSync,
  lstatSync,
  writeFileSync,
  readdirSync,
  readFileSync,
  statSync,
  rmSync,
} from "node:fs";
import { dirname, resolve, join } from "node:path";

import { getLogger, LogLevel, setLogLevel } from "./logging.js";
import {
  CategoryReport,
  TestCaseDefinition,
  TestCaseReport,
  TestCaseType,
  TestReport,
  TestResult,
  UnexecutedReason,
  UnexecutedReasonCode,
} from "./models.js";
import { spawnSync } from "node:child_process";

const logger = getLogger("main");

interface CliArguments {
  tests_dir: string;
  recursive: boolean;
  output: string | null;
  dry_run: boolean;
  include: string[] | null;
  include_category: string[] | null;
  include_test: string[] | null;
  exclude: string[] | null;
  exclude_category: string[] | null;
  exclude_test: string[] | null;
  verbose: number;
  regex_filters: boolean;
  help: boolean;
}

class CliArgumentParsingError extends Error {
  public constructor(message: string) {
    super(message);
    this.name = "CliArgumentParsingError";
  }
}

interface ParserState {
  positionalArguments: string[];
  include: string[];
  includeCategory: string[];
  includeTest: string[];
  exclude: string[];
  excludeCategory: string[];
  excludeTest: string[];
  recursive: boolean;
  output: string | null;
  dryRun: boolean;
  regexFilters: boolean;
  verbose: number;
  help: boolean;
}

function writeResult(resultReport: TestReport, outputFile: string | null): void {
  /**
   * Writes the final report to the specified output file or standard output if no file is provided.
   */
  const resultJson = JSON.stringify(resultReport, null, 2);
  if (outputFile !== null) {
    writeFileSync(outputFile, resultJson, "utf8");
    return;
  }

  console.log(resultJson);
}

function createParserState(): ParserState {
  return {
    positionalArguments: [],
    include: [],
    includeCategory: [],
    includeTest: [],
    exclude: [],
    excludeCategory: [],
    excludeTest: [],
    recursive: false,
    output: null,
    dryRun: false,
    regexFilters: false,
    verbose: 0,
    help: false,
  };
}

function readRequiredValue(
  argv: string[],
  currentIndex: number,
  optionName: string
): [string, number] {
  const value = argv[currentIndex + 1];
  if (value === undefined || value.startsWith("-")) {
    throw new CliArgumentParsingError(`Missing value for ${optionName}`);
  }

  return [value, currentIndex + 2];
}
/*
function collectOptionalValues(argv: string[], startIndex: number): [string[], number] {
  const values: string[] = [];
  let index = startIndex;

  while (index < argv.length) {
    const value = argv[index];
    if (value === undefined || value.startsWith("-")) {
      break;
    }

    values.push(value);
    index += 1;
  }

  return [values, index];
}
*/
function tryConsumeBooleanOption(current: string, state: ParserState): boolean {
  if (current === "-r" || current === "--recursive") {
    state.recursive = true;
    return true;
  }

  if (current === "--dry-run") {
    state.dryRun = true;
    return true;
  }

  if (current === "-g") {
    state.regexFilters = true;
    return true;
  }

  if (current === "-v" || current === "--verbose") {
    state.verbose += 1;
    return true;
  }

  if (current == "-h" || current == "--help") {
    state.help = true;
    return true;
  }

  return false;
}

function tryConsumeRequiredValueOption(
  argv: string[],
  current: string,
  index: number,
  state: ParserState
): number | null {
  // support for flag '--output=<output-path>'
  if (current.startsWith("--output=")) {
    state.output = current.slice("--output=".length);
    return index + 1;
  }

  // support for flag '-o <output-path>' or '--output <output-path>'
  if (current !== "-o" && current !== "--output") {
    return null;
  }

  const [value, nextIndex] = readRequiredValue(argv, index, "--output");
  state.output = value;
  return nextIndex;
}

function tryConsumeSingleListValueOption(
  argv: string[],
  current: string,
  index: number,
  state: ParserState
): number | null {
  if (current !== "-et" && current !== "--exclude-test") {
    return null;
  }

  const [value, nextIndex] = readRequiredValue(argv, index, "--exclude-test");
  state.excludeTest.push(value);
  return nextIndex;
}

const OPTIONAL_LIST_OPTION_NAMES = {
  "-i": "include",
  "--include": "include",
  "-ic": "includeCategory",
  "--include-category": "includeCategory",
  "-it": "includeTest",
  "--include-test": "includeTest",
  "-e": "exclude",
  "--exclude": "exclude",
  "-ec": "excludeCategory",
  "--exclude-category": "excludeCategory",
} as const;

type OptionalListOptionName = keyof typeof OPTIONAL_LIST_OPTION_NAMES;
type OptionalListStateKey = (typeof OPTIONAL_LIST_OPTION_NAMES)[OptionalListOptionName];

function getOptionalListTarget(current: string, state: ParserState): string[] | null {
  const key = OPTIONAL_LIST_OPTION_NAMES[current as OptionalListOptionName] as
    | OptionalListStateKey
    | undefined;
  return key === undefined ? null : state[key];
}

function tryConsumeOptionalListOption(
  argv: string[],
  current: string,
  index: number,
  state: ParserState
): number | null {
  // support for --include=<id> syntax
  for (const [flag, key] of Object.entries(OPTIONAL_LIST_OPTION_NAMES)) {
    if (current.startsWith(flag + "=")) {
      const value = current.slice(flag.length + 1).trim();
      state[key].push(value);
      return index + 1;
    }
  }

  // support for --include <id> syntax
  const target = getOptionalListTarget(current, state);
  if (target === null) {
    return null;
  }

  const [value, nextIndex] = readRequiredValue(argv, index, current);
  target.push(value.trim());
  return nextIndex;
}

type OptionConsumer = (
  argv: string[],
  current: string,
  index: number,
  state: ParserState
) => number | null;

function consumeToken(argv: string[], index: number, state: ParserState): number {
  const current = argv[index];
  if (current === undefined) {
    return index + 1;
  }

  if (tryConsumeBooleanOption(current, state)) {
    return index + 1;
  }

  const consumers: OptionConsumer[] = [
    tryConsumeRequiredValueOption,
    tryConsumeOptionalListOption,
    tryConsumeSingleListValueOption,
  ];
  for (const consumeOption of consumers) {
    const nextIndex = consumeOption(argv, current, index, state);
    if (nextIndex !== null) {
      return nextIndex;
    }
  }

  if (current.startsWith("-")) {
    throw new CliArgumentParsingError(`Unknown argument: ${current}`);
  }

  state.positionalArguments.push(current);
  return index + 1;
}

function nullIfEmpty(arr: string[]): string[] | null {
  return arr.length > 0 ? arr : null;
}

function toArray(arr: string[] | null): string[] {
  return arr !== null ? arr : [];
}

function parseCliArguments(argv: string[]): CliArguments {
  const state = createParserState();

  let index = 0;
  while (index < argv.length) {
    index = consumeToken(argv, index, state);
  }

  if (state.help) {
    return {
      tests_dir: "",
      recursive: false,
      output: null,
      dry_run: false,
      include: null,
      include_category: null,
      include_test: null,
      exclude: null,
      exclude_category: null,
      exclude_test: null,
      verbose: 0,
      regex_filters: false,
      help: true,
    };
  }

  if (state.positionalArguments.length !== 1 || state.positionalArguments[0] === undefined) {
    console.error("Exactly one positional argument (tests_dir) is required.");
    process.exit(2);
  }

  return {
    tests_dir: resolve(state.positionalArguments[0]),
    recursive: state.recursive,
    output: state.output,
    dry_run: state.dryRun,
    include: nullIfEmpty(state.include),
    include_category: nullIfEmpty(state.includeCategory),
    include_test: nullIfEmpty(state.includeTest),
    exclude: nullIfEmpty(state.exclude),
    exclude_category: nullIfEmpty(state.excludeCategory),
    exclude_test: nullIfEmpty(state.excludeTest),
    verbose: state.verbose,
    regex_filters: state.regexFilters,
    help: state.help,
  };
}

function parseArguments(): CliArguments {
  /**
   * Parses the command-line arguments and performs basic validation a sanitization.
   */

  // Parse the provided arguments
  // argparse will automatically print an error message and exit with the return code 2
  // in case of invalid arguments
  let args: CliArguments;
  try {
    args = parseCliArguments(process.argv.slice(2));
  } catch (error) {
    const message = error instanceof Error ? error.message : String(error);
    console.error(message);
    process.exit(2);
  }

  // Check source directory
  if (!args.help && (!existsSync(args.tests_dir) || !lstatSync(args.tests_dir).isDirectory())) {
    console.error("The provided path is not a directory.");
    process.exit(1);
  }

  // Warn if the output file already exists
  if (args.output !== null) {
    const parentDirectory = dirname(args.output);
    if (!existsSync(parentDirectory)) {
      console.error("The parent directory of the output file does not exist.");
      process.exit(1);
    }
    if (existsSync(args.output)) {
      logger.warning("The output file will be overwritten:", args.output);
    }
  }

  return args;
}

function parseTestFile(content: string): {
  description: string | null;
  category: string;
  parserCodes: number[];
  intCodes: number[];
  points: number;
  sourceCode: string;
} {
  const lines = content.split("\n");

  let description: string | null = null;
  let category: string = "";
  const parserCodes: number[] = [];
  const intCodes: number[] = [];
  let points: number = 0;
  let sourceStartIndex = 0;

  for (let i = 0; i < lines.length; i++) {
    const line = (lines[i] ?? "").trim();

    if (line.startsWith("*** ")) {
      description = line.slice(4).trim();
    } else if (line.startsWith("+++ ")) {
      category = line.slice(4).trim();
    } else if (line.startsWith("!C! ")) {
      parserCodes.push(parseInt(line.slice(4).trim(), 10));
    } else if (line.startsWith("!I! ")) {
      intCodes.push(parseInt(line.slice(4).trim(), 10));
    } else if (line.startsWith(">>> ")) {
      points = parseFloat(line.slice(4).trim());
    } else if (line == "") {
      sourceStartIndex = i + 1;
      break;
    }
  }
  const sourceCode = lines.slice(sourceStartIndex).join("\n");

  return { description, category, parserCodes, intCodes, points, sourceCode };
}

function processTestFile(
  path: string,
  file: string,
  result_list: TestCaseDefinition[],
  unexecuted: Record<string, UnexecutedReason>
): void {
  const fullPath = join(path, file);
  const baseName = file.slice(0, -5);
  const content = readFileSync(fullPath, "utf-8");
  const { description, category, parserCodes, intCodes, points } = parseTestFile(content);

  const hasParserCodes = parserCodes.length > 0;
  const hasIntCodes = intCodes.length > 0;

  let test_type: TestCaseType;
  if (hasParserCodes) {
    test_type = hasIntCodes ? TestCaseType.COMBINED : TestCaseType.PARSE_ONLY;
  } else if (hasIntCodes) {
    test_type = TestCaseType.EXECUTE_ONLY;
  } else {
    unexecuted[baseName] = new UnexecutedReason(UnexecutedReasonCode.CANNOT_DETERMINE_TYPE);
    return;
  }

  const stdinPath = join(path, baseName + ".in");
  const stdoutPath = join(path, baseName + ".out");

  try {
    result_list.push(
      new TestCaseDefinition({
        name: baseName,
        test_type,
        description,
        category,
        points,
        test_source_path: fullPath,
        stdin_file: existsSync(stdinPath) ? stdinPath : null,
        expected_stdout_file: existsSync(stdoutPath) ? stdoutPath : null,
        expected_parser_exit_codes:
          test_type === TestCaseType.PARSE_ONLY
            ? parserCodes
            : test_type === TestCaseType.COMBINED
              ? [0]
              : null,
        expected_interpreter_exit_codes: hasIntCodes ? intCodes : null,
      })
    );
  } catch (e) {
    const msg = e instanceof Error ? e.message : String(e);
    unexecuted[baseName] = new UnexecutedReason(
      UnexecutedReasonCode.MALFORMED_TEST_CASE_FILE,
      msg
    );
  }
}

function createTestCaseDefinitions(
  path: string,
  result_list: TestCaseDefinition[],
  unexecuted: Record<string, UnexecutedReason>,
  recursive: boolean
): null {
  const test_files = readdirSync(path);
  for (const file of test_files) {
    const fullPath = join(path, file);
    if (statSync(fullPath).isDirectory()) {
      if (recursive) {
        createTestCaseDefinitions(fullPath, result_list, unexecuted, recursive);
      }
      continue;
    }
    if (!file.endsWith(".test")) {
      continue;
    }
    processTestFile(path, file, result_list, unexecuted);
  }
  return null;
}

// helper for regex handling
function matchesFilter(value: string, filter: string, useRegex: boolean): boolean {
  if (useRegex) {
    try {
      return new RegExp(filter).test(value);
    } catch (e) {
      logger.warning("Invalid regular expression: ", filter, e);
      return false;
    }
  }
  return value === filter;
}

function testIncludeByNameOrCategory(
  testcase: TestCaseDefinition,
  filters: string[],
  testsToRun: TestCaseDefinition[],
  filteredTests: Record<string, UnexecutedReason>,
  useRegex: boolean
): void {
  for (const id of filters) {
    if (
      matchesFilter(testcase.category, id, useRegex) ||
      matchesFilter(testcase.name, id, useRegex)
    ) {
      if (!testsToRun.includes(testcase)) {
        testsToRun.push(testcase);
      }
    } else {
      if (testsToRun.includes(testcase)) return;
      filteredTests[testcase.name] = new UnexecutedReason(
        UnexecutedReasonCode.FILTERED_OUT,
        "Filtered by user"
      );
    }
  }
}

function testIncludeByCategory(
  testcase: TestCaseDefinition,
  filters: string[],
  testsToRun: TestCaseDefinition[],
  filteredTests: Record<string, UnexecutedReason>,
  useRegex: boolean
): void {
  for (const cat_id of filters) {
    if (matchesFilter(testcase.category, cat_id, useRegex)) {
      if (!testsToRun.includes(testcase)) {
        testsToRun.push(testcase);
      }
    } else {
      if (testsToRun.includes(testcase)) return;
      filteredTests[testcase.name] = new UnexecutedReason(
        UnexecutedReasonCode.FILTERED_OUT,
        "Filtered by user"
      );
    }
  }
}

function testIncludeByName(
  testcase: TestCaseDefinition,
  filters: string[],
  testsToRun: TestCaseDefinition[],
  filteredTests: Record<string, UnexecutedReason>,
  useRegex: boolean
): void {
  for (const name_id of filters) {
    if (matchesFilter(testcase.name, name_id, useRegex)) {
      if (!testsToRun.includes(testcase)) {
        testsToRun.push(testcase);
      }
    } else {
      if (testsToRun.includes(testcase)) return;
      filteredTests[testcase.name] = new UnexecutedReason(
        UnexecutedReasonCode.FILTERED_OUT,
        "Filtered by user"
      );
    }
  }
}

function testExcludeByNameOrCategory(
  testcase: TestCaseDefinition,
  filters: string[],
  testsToRun: TestCaseDefinition[],
  filteredTests: Record<string, UnexecutedReason>,
  useRegex: boolean
): void {
  for (const id of filters) {
    if (
      matchesFilter(testcase.category, id, useRegex) ||
      matchesFilter(testcase.name, id, useRegex)
    ) {
      const idx = testsToRun.indexOf(testcase);
      if (idx !== -1) testsToRun.splice(idx, 1);
      filteredTests[testcase.name] = new UnexecutedReason(
        UnexecutedReasonCode.FILTERED_OUT,
        "Filtered by user"
      );
    }
  }
}

function testExcludeByCategory(
  testcase: TestCaseDefinition,
  filters: string[],
  testsToRun: TestCaseDefinition[],
  filteredTests: Record<string, UnexecutedReason>,
  useRegex: boolean
): void {
  for (const cat_id of filters) {
    if (matchesFilter(testcase.category, cat_id, useRegex)) {
      const idx = testsToRun.indexOf(testcase);
      if (idx !== -1) testsToRun.splice(idx, 1);
      filteredTests[testcase.name] = new UnexecutedReason(
        UnexecutedReasonCode.FILTERED_OUT,
        "Filtered by user"
      );
    }
  }
}

function testExcludeByName(
  testcase: TestCaseDefinition,
  filters: string[],
  testsToRun: TestCaseDefinition[],
  filteredTests: Record<string, UnexecutedReason>,
  useRegex: boolean
): void {
  for (const name_id of filters) {
    if (matchesFilter(testcase.name, name_id, useRegex)) {
      const idx = testsToRun.indexOf(testcase);
      if (idx !== -1) testsToRun.splice(idx, 1);
      filteredTests[testcase.name] = new UnexecutedReason(
        UnexecutedReasonCode.FILTERED_OUT,
        "Filtered by user"
      );
    }
  }
}

function filterTestsByUser(
  discovered_tests: TestCaseDefinition[],
  tests_to_run: TestCaseDefinition[],
  filtered_tests: Record<string, UnexecutedReason>,
  include: string[] | null,
  include_category: string[] | null,
  include_test: string[] | null,
  exclude: string[] | null,
  exclude_category: string[] | null,
  exclude_test: string[] | null,
  use_regex: boolean
): null {
  // test including criteria
  if (!include && !include_category && !include_test) {
    tests_to_run.push(...discovered_tests);
  } else {
    for (const testcase of discovered_tests) {
      // include by name OR category
      testIncludeByNameOrCategory(
        testcase,
        toArray(include),
        tests_to_run,
        filtered_tests,
        use_regex
      );

      // include by category
      testIncludeByCategory(
        testcase,
        toArray(include_category),
        tests_to_run,
        filtered_tests,
        use_regex
      );

      // include by name
      testIncludeByName(testcase, toArray(include_test), tests_to_run, filtered_tests, use_regex);
    }
  }

  // test excluding criteria
  for (const testcase of discovered_tests) {
    if (!exclude && !exclude_category && !exclude_test) break;
    // exclude by name OR category
    testExcludeByNameOrCategory(
      testcase,
      toArray(exclude),
      tests_to_run,
      filtered_tests,
      use_regex
    );

    // exclude by category
    testExcludeByCategory(
      testcase,
      toArray(exclude_category),
      tests_to_run,
      filtered_tests,
      use_regex
    );

    // exclude by name
    testExcludeByName(testcase, toArray(exclude_test), tests_to_run, filtered_tests, use_regex);
  }
  return null;
}

type CategoryReports = Record<string, [number, number, Record<string, TestCaseReport>]>;

function recordResult(
  category_reports: CategoryReports,
  test: TestCaseDefinition,
  res: TestResult,
  caseReport: TestCaseReport
): void {
  const cat = category_reports[test.category];
  if (!cat) return;
  cat[0] += test.points;
  if (res === TestResult.PASSED) cat[1] += test.points;
  cat[2][test.name] = caseReport;
}

function runTestParseOnly(category_reports: CategoryReports, test: TestCaseDefinition): void {
  const content = readFileSync(test.test_source_path, "utf8");
  const { sourceCode } = parseTestFile(content);

  const tmpSol = join(dirname(test.test_source_path), test.name + "_tmp.sol");
  writeFileSync(tmpSol, sourceCode);

  const output = spawnSync("python", ["sol2xml/sol_to_xml.py", tmpSol]);
  rmSync(tmpSol);

  const parserExitCode = output.status ?? 0;
  const parserStderr = output.stderr.toString();
  const parserStdout = output.stdout.toString();

  const res = test.expected_parser_exit_codes?.includes(parserExitCode)
    ? TestResult.PASSED
    : TestResult.UNEXPECTED_PARSER_EXIT_CODE;

  const caseReport = new TestCaseReport(
    res,
    parserExitCode,
    null,
    parserStdout,
    parserStderr,
    null,
    null,
    null
  );
  recordResult(category_reports, test, res, caseReport);
}

function runTestIntOnly(category_reports: CategoryReports, test: TestCaseDefinition) {
  const content = readFileSync(test.test_source_path, "utf8");
  const { sourceCode } = parseTestFile(content);

  const tmpXml = join(dirname(test.test_source_path), test.name + "_tmp.xml");
  writeFileSync(tmpXml, sourceCode);

  const intOutput =
    test.stdin_file !== null
      ? spawnSync("python", ["/interpreter/src/solint.py", "-s", tmpXml, "-i", test.stdin_file])
      : spawnSync("python", ["/interpreter/src/solint.py", "-s", tmpXml]);

  rmSync(tmpXml);

  const intExitCode = intOutput.status ?? 0;
  const intStderr = intOutput.stderr.toString();
  const intStdout = intOutput.stdout.toString();

  let res: TestResult;
  let compareOutput: string | null = null;

  if (test.expected_interpreter_exit_codes?.includes(intExitCode)) {
    if (test.expected_stdout_file !== null) {
      const tmpFile = join(dirname(test.test_source_path), test.name + "_actual.out");
      writeFileSync(tmpFile, intStdout);

      const compare = spawnSync("diff", [tmpFile, test.expected_stdout_file]);
      compareOutput = compare.stdout.toString();
      rmSync(tmpFile);
      res = compare.status === 0 ? TestResult.PASSED : TestResult.INTERPRETER_RESULT_DIFFERS;
    } else {
      res = TestResult.PASSED;
    }
  } else {
    res = TestResult.UNEXPECTED_INTERPRETER_EXIT_CODE;
  }

  const caseReport = new TestCaseReport(
    res,
    null,
    intExitCode,
    null,
    null,
    intStdout,
    intStderr,
    compareOutput
  );
  recordResult(category_reports, test, res, caseReport);
}

function runTestCombined(category_reports: CategoryReports, test: TestCaseDefinition) {
  // first compile from .sol -> .xml

  const content = readFileSync(test.test_source_path, "utf8");
  const { sourceCode } = parseTestFile(content);

  const tmpSol = join(dirname(test.test_source_path), test.name + "_tmp.sol");
  writeFileSync(tmpSol, sourceCode);

  const xmlOutput = spawnSync("python", ["sol2xml/sol_to_xml.py", tmpSol]);
  rmSync(tmpSol);

  const parserExitCode = xmlOutput.status ?? 0;
  const parserStderr = xmlOutput.stderr.toString();
  const parserStdout = xmlOutput.stdout.toString();

  const parserOk = parserExitCode === 0;

  const tmpXml = join(dirname(test.test_source_path), test.name + "_parsed.xml");
  writeFileSync(tmpXml, parserStdout);

  // then run interpreter
  const intOutput =
    test.stdin_file !== null
      ? spawnSync("python", ["/interpreter/src/solint.py", "-s", tmpXml, "-i", test.stdin_file])
      : spawnSync("python", ["/interpreter/src/solint.py", "-s", tmpXml]);

  rmSync(tmpXml);

  const intExitCode = intOutput.status ?? 0;
  const intStderr = intOutput.stderr.toString();
  const intStdout = intOutput.stdout.toString();

  // determine result
  let res: TestResult;
  let compareOutput: string | null = null;

  if (!parserOk) {
    res = TestResult.UNEXPECTED_PARSER_EXIT_CODE;
  } else if (test.expected_interpreter_exit_codes?.includes(intExitCode)) {
    if (test.expected_stdout_file !== null) {
      const tmpFile = join(dirname(test.test_source_path), test.name + "_actual.out");
      writeFileSync(tmpFile, intStdout);
      const compare = spawnSync("diff", [tmpFile, test.expected_stdout_file]);
      compareOutput = compare.stdout.toString();
      rmSync(tmpFile);
      res = compare.status === 0 ? TestResult.PASSED : TestResult.INTERPRETER_RESULT_DIFFERS;
    } else {
      res = TestResult.PASSED;
    }
  } else {
    res = TestResult.UNEXPECTED_INTERPRETER_EXIT_CODE;
  }

  const caseReport = new TestCaseReport(
    res,
    parserExitCode,
    intExitCode,
    parserStdout,
    parserStderr,
    intStdout,
    intStderr,
    compareOutput
  );
  recordResult(category_reports, test, res, caseReport);
}

function buildCategoryMap(tests_to_run: TestCaseDefinition[]): CategoryReports {
  const category_reports: CategoryReports = {};
  for (const test of tests_to_run) {
    if (!(test.category in category_reports)) {
      category_reports[test.category] = [0, 0, {}];
    }
  }
  return category_reports;
}

function executeTests(
  tests_to_run: TestCaseDefinition[],
  category_reports: CategoryReports,
  dry_run: boolean
): void {
  if (dry_run) {
    return;
  }
  for (const test of tests_to_run) {
    if (test.test_type === TestCaseType.PARSE_ONLY) {
      runTestParseOnly(category_reports, test);
    }
    if (test.test_type === TestCaseType.EXECUTE_ONLY) {
      runTestIntOnly(category_reports, test);
    }
    if (test.test_type === TestCaseType.COMBINED) {
      runTestCombined(category_reports, test);
    }
  }
}
/*
function printSummary(category_reports: CategoryReports): void {
  const GREEN = "\x1b[32m";
  const RED = "\x1b[31m";
  const RESET = "\x1b[0m";
  const BOLD = "\x1b[1m";

  console.error(`\n${BOLD}=== TEST SUMMARY ===${RESET}`);
  for (const [category, [total, passed, results]] of Object.entries(category_reports)) {
    console.error(`\n${BOLD}${category}${RESET} (${String(passed)}/${String(total)} pts)`);
    for (const [testName, report] of Object.entries(results)) {
      const color = report.result === TestResult.PASSED ? GREEN : RED;
      console.error(`  ${color}${report.result}${RESET}  ${testName}`);
    }
  }
  let totalPts = 0;
  let totalPassed = 0;
  for (const [, [total, passed]] of Object.entries(category_reports)) {
    totalPts += total;
    totalPassed += passed;
  }
  const totalColor = totalPassed === totalPts ? GREEN : RED;
  console.error(
    `${BOLD}TOTAL: ${totalColor}${String(totalPassed)}/${String(totalPts)} pts${RESET}\n`
  );
}
*/
function main(): void {
  /**
   * The main entry point for the SOL26 integration testing script.
   * It parses command-line arguments and executes the testing process.
   */

  // Set up logging
  // IPP: You do not have to use logging - but it is the recommended practice.
  setLogLevel(LogLevel.WARNING);

  // Parse the CLI arguments
  const args = parseArguments();

  // Enable debug or info logging if the verbose flag was set twice or once
  if (args.verbose >= 2) {
    setLogLevel(LogLevel.DEBUG);
  } else if (args.verbose === 1) {
    setLogLevel(LogLevel.INFO);
  }

  // TODO: Your code for discovering and executing the test cases goes here.
  // --help flag handling
  if (args.help) {
    console.log(
      "\n-----------------------------------TESTER USAGE--------------------------------------------------------------------\n"
    );
    console.log("node dist/tester.js < -h / --help>         Prints this guide.");
    console.log(
      "node dist/tester.js <tests-dir> [OPTIONS]  Starts the tester with files from specified testdir.\n"
    );
    console.log("OPTIONS: ");
    console.log(
      " --recursive / -r                    Tester recursively scans files also in subdirectories of specified <test-dir>"
    );
    console.log(
      " --output=<file> / -o <file>         Sets path to output file that the final report will be written to"
    );
    console.log(
      "                                     If no file is provided, report will be printed to stdout."
    );
    console.log(
      " --dry-run                           Test files will be loaded but no test will be ran."
    );
    console.log(
      " --include=<id> / -i <id>            Only tests whose name or category matches with 'id' will run."
    );
    console.log(
      " --exclude=<id> / -e <id>            All tests will run, excluding those whose name or category matches with 'id'.\n"
    );
    console.log(
      " --include-category=<id> / -ic <id>  Only tests from 'id'-named category will run."
    );
    console.log(" --include-test=<id> / -it <id>      Only test with 'id' name will run.\n");
    console.log(
      " --exclude-category=<id / -ec <id>   All tests without ones from 'id'-named category will run."
    );
    console.log(
      " --exclude-test=<id> / -et <id>      All tests without the one with 'id' name will run.\n"
    );

    console.log(
      " -g                                  When used, the filters specified with -i[ct]/-e[ct] will"
    );
    console.log(
      "                                     be interpreted as regular expressions instead of literal strings."
    );
    console.log(
      " -v, --verbose                       Enable verbose logging output (using once = INFO level, using twice = DEBUG level)."
    );
    console.log(
      "------------------------------------------------------------------------------------------------------------------\n"
    );
    return;
  }

  const discovered_tests: TestCaseDefinition[] = [];
  const tests_to_run: TestCaseDefinition[] = [];
  const filtered_tests: Record<string, UnexecutedReason> = {};

  createTestCaseDefinitions(args.tests_dir, discovered_tests, filtered_tests, args.recursive);
  // filter testcases based on other CLI arguments
  filterTestsByUser(
    discovered_tests,
    tests_to_run,
    filtered_tests,
    args.include,
    args.include_category,
    args.include_test,
    args.exclude,
    args.exclude_category,
    args.exclude_test,
    args.regex_filters
  );

  const category_reports = buildCategoryMap(tests_to_run);

  // TestResult --> TestCaseReport --> TestCategoryReport --> TestReport
  executeTests(tests_to_run, category_reports, args.dry_run);

  const final_results: Record<string, CategoryReport> = {};
  for (const [key, [total, passed, results]] of Object.entries(category_reports)) {
    const new_cat_report = new CategoryReport(total, passed, results);
    final_results[key] = new_cat_report;
  }

  // print colored summary
  // printSummary(category_reports);

  // Example of how to write the final report:
  const report = new TestReport({
    discovered_test_cases: discovered_tests,
    unexecuted: filtered_tests,
    results: args.dry_run ? null : final_results,
  });
  writeResult(report, args.output);
}

// keep imports available for step 5 implementation
void (TestCaseReport satisfies unknown);
void (TestResult satisfies unknown);

main();
