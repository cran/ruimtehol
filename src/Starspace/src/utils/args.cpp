/**
 * Copyright (c) 2016-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#include "args.h"

#include <iostream>
#include <algorithm>
#include <string>
#include <cstring>
#include <assert.h>
#include <Rcpp.h>

using namespace std;

namespace starspace {

Args::Args() {
  lr = 0.01;
  termLr = 1e-9;
  norm = 1.0;
  margin = 0.05;
  wordWeight = 0.5;
  initRandSd = 0.001;
  dropoutLHS = 0.0;
  dropoutRHS = 0.0;
  p = 0.5;
  dim = 100;
  epoch = 5;
  ws = 5;
  maxTrainTime = 60*60*24*100;
  validationPatience = 10;
  thread = 10;
  maxNegSamples = 10;
  negSearchLimit = 50;
  minCount = 1;
  minCountLabel = 1;
  K = 5;
  verbose = false;
  debug = false;
  adagrad = true;
  normalizeText = false;
  trainMode = 0;
  fileFormat = "fastText";
  label = "__label__";
  bucket = 2000000;
  ngrams = 1;
  loss = "hinge";
  similarity = "cosine";
  isTrain = false;
  shareEmb = true;
  saveEveryEpoch = false;
  saveTempModel = false;
  useWeight = false;
  trainWord = false;
  excludeLHS = false;
}

bool Args::isTrue(string arg) {
  std::transform(arg.begin(), arg.end(), arg.begin(),
      [&](char c) { return tolower(c); }
  );
  return (arg == "true" || arg == "1");
}

void Args::parseArgs(int argc, char** argv) {
  if (argc <= 1) {
    Rcpp::Rcerr << "Usage: need to specify whether it is train or test.\n";
    printHelp();
    Rcpp::stop("Incorrect Starspace usage");
  }
  if (strcmp(argv[1], "train") == 0) {
    isTrain = true;
  } else if (strcmp(argv[1], "test") == 0) {
    isTrain = false;
  } else if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "-help") == 0) {
    Rcpp::Rcerr << "Here is the help! Usage:" << std::endl;
    printHelp();
    Rcpp::stop("Incorrect Starspace usage");
  } else {
    Rcpp::Rcerr << "Usage: the first argument should be either train or test.\n";
    printHelp();
    Rcpp::stop("Incorrect Starspace usage");
  }
  int i = 2;
  while (i < argc) {
    if (argv[i][0] != '-') {
      Rcpp::Rcout << "Provided argument without a dash! Usage:" << endl;
      printHelp();
      Rcpp::stop("Incorrect Starspace usage");
    }

    // handling "--"
    if (strlen(argv[i]) >= 2 && argv[i][1] == '-') {
      argv[i] = argv[i] + 1;
    }

    if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "-help") == 0) {
      Rcpp::Rcerr << "Here is the help! Usage:" << std::endl;
      printHelp();
      Rcpp::stop("Incorrect Starspace usage");
    } else if (strcmp(argv[i], "-trainFile") == 0) {
      trainFile = string(argv[i + 1]);
    } else if (strcmp(argv[i], "-validationFile") == 0) {
      validationFile = string(argv[i + 1]);
    } else if (strcmp(argv[i], "-testFile") == 0) {
      testFile = string(argv[i + 1]);
    } else if (strcmp(argv[i], "-predictionFile") == 0) {
      predictionFile = string(argv[i + 1]);
    } else if (strcmp(argv[i], "-basedoc") == 0) {
      basedoc = string(argv[i + 1]);
    } else if (strcmp(argv[i], "-model") == 0) {
      model = string(argv[i + 1]);
    } else if (strcmp(argv[i], "-initModel") == 0) {
      initModel = string(argv[i + 1]);
    } else if (strcmp(argv[i], "-fileFormat") == 0) {
      fileFormat = string(argv[i + 1]);
    } else if (strcmp(argv[i], "-label") == 0) {
      label = string(argv[i + 1]);
    } else if (strcmp(argv[i], "-loss") == 0) {
      loss = string(argv[i + 1]);
    } else if (strcmp(argv[i], "-similarity") == 0) {
      similarity = string(argv[i + 1]);
    } else if (strcmp(argv[i], "-lr") == 0) {
      lr = atof(argv[i + 1]);
    } else if (strcmp(argv[i], "-p") == 0) {
      p = atof(argv[i + 1]);
    } else if (strcmp(argv[i], "-termLr") == 0) {
      termLr = atof(argv[i + 1]);
    } else if (strcmp(argv[i], "-norm") == 0) {
      norm = atof(argv[i + 1]);
    } else if (strcmp(argv[i], "-margin") == 0) {
      margin = atof(argv[i + 1]);
    } else if (strcmp(argv[i], "-initRandSd") == 0) {
      initRandSd = atof(argv[i + 1]);
    } else if (strcmp(argv[i], "-dropoutLHS") == 0) {
      dropoutLHS = atof(argv[i + 1]);
    } else if (strcmp(argv[i], "-dropoutRHS") == 0) {
      dropoutRHS = atof(argv[i + 1]);
    } else if (strcmp(argv[i], "-wordWeight") == 0) {
      wordWeight = atof(argv[i + 1]);
    } else if (strcmp(argv[i], "-dim") == 0) {
      dim = atoi(argv[i + 1]);
    } else if (strcmp(argv[i], "-epoch") == 0) {
      epoch = atoi(argv[i + 1]);
    } else if (strcmp(argv[i], "-ws") == 0) {
      ws = atoi(argv[i + 1]);
    } else if (strcmp(argv[i], "-maxTrainTime") == 0) {
      maxTrainTime = atoi(argv[i + 1]);
    } else if (strcmp(argv[i], "-validationPatience") == 0) {
      validationPatience = atoi(argv[i + 1]);
    } else if (strcmp(argv[i], "-thread") == 0) {
      thread = atoi(argv[i + 1]);
    } else if (strcmp(argv[i], "-maxNegSamples") == 0) {
      maxNegSamples = atoi(argv[i + 1]);
    } else if (strcmp(argv[i], "-negSearchLimit") == 0) {
      negSearchLimit = atoi(argv[i + 1]);
    } else if (strcmp(argv[i], "-minCount") == 0) {
      minCount = atoi(argv[i + 1]);
    } else if (strcmp(argv[i], "-minCountLabel") == 0) {
      minCountLabel = atoi(argv[i + 1]);
    } else if (strcmp(argv[i], "-bucket") == 0) {
      bucket = atoi(argv[i + 1]);
    } else if (strcmp(argv[i], "-ngrams") == 0) {
      ngrams = atoi(argv[i + 1]);
    } else if (strcmp(argv[i], "-K") == 0) {
      K = atoi(argv[i + 1]);
    } else if (strcmp(argv[i], "-trainMode") == 0) {
      trainMode = atoi(argv[i + 1]);
    } else if (strcmp(argv[i], "-verbose") == 0) {
      verbose = isTrue(string(argv[i + 1]));
    } else if (strcmp(argv[i], "-debug") == 0) {
      debug = isTrue(string(argv[i + 1]));
    } else if (strcmp(argv[i], "-adagrad") == 0) {
      adagrad = isTrue(string(argv[i + 1]));
    } else if (strcmp(argv[i], "-shareEmb") == 0) {
      shareEmb = isTrue(string(argv[i + 1]));
    } else if (strcmp(argv[i], "-normalizeText") == 0) {
      normalizeText = isTrue(string(argv[i + 1]));
    } else if (strcmp(argv[i], "-saveEveryEpoch") == 0) {
      saveEveryEpoch = isTrue(string(argv[i + 1]));
    } else if (strcmp(argv[i], "-saveTempModel") == 0) {
      saveTempModel = isTrue(string(argv[i + 1]));
    } else if (strcmp(argv[i], "-useWeight") == 0) {
      useWeight = isTrue(string(argv[i + 1]));
    } else if (strcmp(argv[i], "-trainWord") == 0) {
      trainWord = isTrue(string(argv[i + 1]));
    } else if (strcmp(argv[i], "-excludeLHS") == 0) {
      excludeLHS = isTrue(string(argv[i + 1]));
    } else {
      Rcpp::Rcerr << "Unknown argument: " << argv[i] << std::endl;
      printHelp();
      Rcpp::stop("Incorrect Starspace usage");
    }
    i += 2;
  }
  if (isTrain) {
    if (trainFile.empty() || model.empty()) {
      Rcpp::Rcerr << "Empty train file or output model path." << endl;
      printHelp();
      Rcpp::stop("Incorrect Starspace usage");
    }
  } else {
    if (testFile.empty() || model.empty()) {
      Rcpp::Rcerr << "Empty test file or model path." << endl;
      printHelp();
      Rcpp::stop("Incorrect Starspace usage");
    }
  }
  // check for trainMode
  if ((trainMode < 0) || (trainMode > 5)) {
    Rcpp::Rcerr << "Uknown trainMode. We currently support the follow train modes:\n";
    Rcpp::Rcerr << "trainMode 0: at training time, one label from RHS is picked as true label; LHS is the same from input.\n";
    Rcpp::Rcerr << "trainMode 1: at training time, one label from RHS is picked as true label; LHS is the bag of the rest RHS labels.\n";
    Rcpp::Rcerr << "trainMode 2: at training time, one label from RHS is picked as LHS; the bag of the rest RHS labels becomes the true label.\n";
    Rcpp::Rcerr << "trainMode 3: at training time, one label from RHS is picked as true label and another label from RHS is picked as LHS.\n";
    Rcpp::Rcerr << "trainMode 4: at training time, the first label from RHS is picked as LHS and the second one picked as true label.\n";
    Rcpp::Rcerr << "trainMode 5: continuous bag of words training.\n";
    Rcpp::stop("Incorrect Starspace usage");
  }
  // check for loss type
  if (!(loss == "hinge" || loss == "softmax")) {
    Rcpp::Rcerr << "Unsupported loss type: " << loss << endl;
    Rcpp::stop("Incorrect Starspace usage");
  }
  // check for similarity type
  if (!(similarity == "cosine" || similarity == "dot")) {
    Rcpp::Rcerr << "Unsupported similarity type. Should be either dot or cosine.\n";
    Rcpp::stop("Incorrect Starspace usage");
  }
  // check for file format
  if (!(fileFormat == "fastText" || fileFormat == "labelDoc")) {
    Rcpp::Rcerr << "Unsupported file format type. Should be either fastText or labelDoc.\n";
    Rcpp::stop("Incorrect Starspace usage");
  }
}

void Args::printHelp() {
  Rcpp::Rcout << "\n"
       << "\"starspace train ...\"  or \"starspace test ...\"\n\n"
       << "The following arguments are mandatory for train: \n"
       << "  -trainFile       training file path\n"
       << "  -model           output model file path\n\n"
       << "The following arguments are mandatory for test: \n"
       << "  -testFile        test file path\n"
       << "  -model           model file path\n\n"
       << "The following arguments for the dictionary are optional:\n"
       << "  -minCount        minimal number of word occurences [" << minCount << "]\n"
       << "  -minCountLabel   minimal number of label occurences [" << minCountLabel << "]\n"
       << "  -ngrams          max length of word ngram [" << ngrams << "]\n"
       << "  -bucket          number of buckets [" << bucket << "]\n"
       << "  -label           labels prefix [" << label << "]\n"
       << "\nThe following arguments for training are optional:\n"
       << "  -initModel       if not empty, it loads a previously trained model in -initModel and carry on training.\n"
       << "  -trainMode       takes value in [0, 1, 2, 3, 4, 5], see Training Mode Section. [" << trainMode << "]\n"
       << "  -fileFormat      currently support 'fastText' and 'labelDoc', see File Format Section. [" << fileFormat << "]\n"
       << "  -validationFile  validation file path\n"
       << "  -validationPatience    number of iterations of validation where does not improve before we stop training [" << validationPatience << "]\n"
       << "  -saveEveryEpoch  save intermediate models after each epoch [" << saveEveryEpoch << "]\n"
       << "  -saveTempModel   save intermediate models after each epoch with an unique name including epoch number [" << saveTempModel << "]\n"
       << "  -lr              learning rate [" << lr << "]\n"
       << "  -dim             size of embedding vectors [" << dim << "]\n"
       << "  -epoch           number of epochs [" << epoch << "]\n"
       << "  -maxTrainTime    max train time (secs) [" << maxTrainTime << "]\n"
       << "  -negSearchLimit  number of negatives sampled [" << negSearchLimit << "]\n"
       << "  -maxNegSamples   max number of negatives in a batch update [" << maxNegSamples << "]\n"
       << "  -loss            loss function {hinge, softmax} [hinge]\n"
       << "  -margin          margin parameter in hinge loss. It's only effective if hinge loss is used. [" << margin << "]\n"
       << "  -similarity      takes value in [cosine, dot]. Whether to use cosine or dot product as similarity function in  hinge loss.\n"
       << "                   It's only effective if hinge loss is used. [" << similarity << "]\n"
       << "  -adagrad         whether to use adagrad in training [" << adagrad << "]\n"
       << "  -shareEmb        whether to use the same embedding matrix for LHS and RHS. [" << shareEmb << "]\n"
       << "  -ws              only used in trainMode 5, the size of the context window for word level training. [" << ws << "]\n"
       << "  -dropoutLHS      dropout probability for LHS features. [" << dropoutLHS << "]\n"
       << "  -dropoutRHS      dropout probability for RHS features. [" << dropoutRHS << "]\n"
       << "  -initRandSd      initial values of embeddings are randomly generated from normal distribution with mean=0, standard deviation=initRandSd. [" << initRandSd << "]\n"
       << "  -trainWord       whether to train word level together with other tasks (for multi-tasking). [" << trainWord << "]\n"
       << "  -wordWeight      if trainWord is true, wordWeight specifies example weight for word level training examples. [" << wordWeight << "]\n"
       << "\nThe following arguments for test are optional:\n"
       << "  -basedoc         file path for a set of labels to compare against true label. It is required when -fileFormat='labelDoc'.\n"
       << "                   In the case -fileFormat='fastText' and -basedoc is not provided, we compare true label with all other labels in the dictionary.\n"
       << "  -predictionFile  file path for save predictions. If not empty, top K predictions for each example will be saved.\n"
       << "  -K               if -predictionFile is not empty, top K predictions for each example will be saved.\n"
       << "  -excludeLHS      exclude elements in the LHS from predictions\n"
       <<  "\nThe following arguments are optional:\n"
       << "  -normalizeText   whether to run basic text preprocess for input files [" << normalizeText << "]\n"
       << "  -useWeight       whether input file contains weights [" << useWeight << "]\n"
       << "  -verbose         verbosity level [" << verbose << "]\n"
       << "  -debug           whether it's in debug mode [" << debug << "]\n"
       << "  -thread          number of threads [" << thread << "]\n"
       << std::endl;
}

void Args::printArgs() {
  Rcpp::Rcout << "Arguments: \n"
       << "lr: " << lr << endl
       << "dim: " << dim << endl
       << "epoch: " << epoch << endl
       << "maxTrainTime: " << maxTrainTime << endl
       << "validationPatience: " << validationPatience << endl
       << "saveEveryEpoch: " << saveEveryEpoch << endl
       << "loss: " << loss << endl
       << "margin: " << margin << endl
       << "similarity: " << similarity << endl
       << "maxNegSamples: " << maxNegSamples << endl
       << "negSearchLimit: " << negSearchLimit << endl
       << "thread: " << thread << endl
       << "minCount: " << minCount << endl
       << "minCountLabel: " << minCountLabel << endl
       << "label: " << label << endl
       << "ngrams: " << ngrams << endl
       << "bucket: " << bucket << endl
       << "adagrad: " << adagrad << endl
       << "trainMode: " << trainMode << endl
       << "fileFormat: " << fileFormat << endl
       << "normalizeText: " << normalizeText << endl
       << "dropoutLHS: " << dropoutLHS << endl
       << "dropoutRHS: " << dropoutRHS << endl;
}

void Args::save(std::ostream& out) {
  out.write((char*) &(dim), sizeof(int));
  out.write((char*) &(epoch), sizeof(int));
  // out.write((char*) &(maxTrainTime), sizeof(int));
  out.write((char*) &(minCount), sizeof(int));
  out.write((char*) &(minCountLabel), sizeof(int));
  out.write((char*) &(maxNegSamples), sizeof(int));
  out.write((char*) &(negSearchLimit), sizeof(int));
  out.write((char*) &(ngrams), sizeof(int));
  out.write((char*) &(bucket), sizeof(int));
  out.write((char*) &(trainMode), sizeof(int));
  out.write((char*) &(shareEmb), sizeof(bool));
  out.write((char*) &(useWeight), sizeof(bool));
  size_t size = fileFormat.size();
  out.write((char*) &(size), sizeof(size_t));
  out.write((char*) &(fileFormat[0]), size);
  size = similarity.size();
  out.write((char*) &(size), sizeof(size_t));
  out.write((char*) &(similarity[0]), size);
}

void Args::load(std::istream& in) {
  in.read((char*) &(dim), sizeof(int));
  in.read((char*) &(epoch), sizeof(int));
  // in.read((char*) &(maxTrainTime), sizeof(int));
  in.read((char*) &(minCount), sizeof(int));
  in.read((char*) &(minCountLabel), sizeof(int));
  in.read((char*) &(maxNegSamples), sizeof(int));
  in.read((char*) &(negSearchLimit), sizeof(int));
  in.read((char*) &(ngrams), sizeof(int));
  in.read((char*) &(bucket), sizeof(int));
  in.read((char*) &(trainMode), sizeof(int));
  in.read((char*) &(shareEmb), sizeof(bool));
  in.read((char*) &(useWeight), sizeof(bool));
  size_t size;
  in.read((char*) &(size), sizeof(size_t));
  fileFormat.resize(size);
  in.read((char*) &(fileFormat[0]), size);
  in.read((char*) &(size), sizeof(size_t));
  similarity.resize(size);
  in.read((char*) &(similarity[0]), size);
}

}
