#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <array>
#include <cmath>
#include <thread>
using namespace std;

vector<string> split(string str, string character){
    vector<string> result;
    long long s=0;
    long long i=0;
    while(i<str.length()){
        if(str[i]==character[0]||i==str.length()-1){
            long long x=i-s;
            if(i==str.length()-1){
                x++;
            }
            result.push_back(str.substr(s,x));
            s=i+1;
        }
        i++;
    }
    return result;
}

struct Parameters{
    float rate;
    int epoch;
    float error;
};

struct Node{
    vector<float> weights;
    float output;
    float delta;
};

struct Case{
    vector<float> outputs;
    int answer;
};

class NeuralNet{
    public:
        vector<vector<Node>> model;
        vector<float> outputSet;
        NeuralNet(vector<vector<float>> dataset,vector<int> layout,Parameters params){
            float rate=params.rate;
            int epoch=params.epoch;
            float error=params.error;
            set<float> outputTypes;
            for(int i=0;i<dataset.size();i++){
                outputTypes.insert(dataset[i][dataset[i].size()-1]);
            }
            int inputSize=dataset[0].size()-1;
            int outputSize=outputTypes.size();
            int hiddenCount=layout.size();
            vector<int> hiddenSize=layout;
            outputSet=vector<float>(outputTypes.size());
            copy(outputTypes.begin(), outputTypes.end(), outputSet.begin());
            for(int i=0;i<dataset.size();i++){
                dataset[i][dataset[i].size()-1]=distance(outputSet.begin(),find(outputSet.begin(),outputSet.end(),dataset[i][dataset[i].size()-1]));
            }
            initNet(inputSize,hiddenCount,hiddenSize,outputSize);
            trainNet(dataset,rate,epoch,error,outputSize);
        }
        NeuralNet(){

        }
        float activate(vector<float> weights, vector<float> inputs){
            float activation=weights[weights.size()-1];
            for(int i=0;i<weights.size()-1;i++){
                activation+=weights[i] * inputs[i];
            }
            return activation;
        }

        float transfer(float activation){
            return 1/(1+exp(-activation));
        }

        float derivative(float out){
            return out*(1-out);
        }

        vector<float> forwardPropagate(vector<float> inputs){
            for(int x=0;x<model.size();x++){
                vector<float> newInputs;
                for(int y=0;y<model[x].size();y++){
                    float activation=activate(model[x][y].weights,inputs);
                    model[x][y].output=transfer(activation);
                    newInputs.push_back(model[x][y].output);
                }
                inputs=newInputs;
            }
            return inputs;
        }

        void backwardPropagate(vector<float> expected){
            for(int x=model.size()-1;x>=0;x--){
                vector<float> errors;
                if(x!=model.size()-1){
                    for(int y=0;y<model[x].size();y++){
                        float error=0;
                        for(int k=0;k<model[x+1].size();k++){
                            error+=model[x+1][k].weights[y]*model[x+1][k].delta;
                        }
                        errors.push_back(error);
                    }
                }else{
                    for(int y=0;y<model[x].size();y++){
                        errors.push_back(expected[y]-model[x][y].output);
                    }
                }
                for(int y=0;y<model[x].size();y++){
                    model[x][y].delta=errors[y]*derivative(model[x][y].output);
                }
            }
        }

        float randomFloat(){
            float randomNum=float(rand()%10000000)/5000000-1;
            return randomNum;
        }

        void initNet(int inputSize, int hiddenCount, vector<int> hiddenSize, int outputSize){
            vector<vector<Node>> hiddenLayers;
            for(int layer=0;layer<hiddenCount;layer++){
                hiddenLayers.push_back({});
                for(int i=0;i<hiddenSize[layer];i++){
                    hiddenLayers[layer].push_back(Node());
                    for(int k=0;k<=inputSize;k++){
                        hiddenLayers[layer][i].weights.push_back(randomFloat());
                    }
                }
                inputSize=hiddenSize[layer];
            }
            for(int i=0;i<hiddenLayers.size();i++){
                model.push_back(hiddenLayers[i]);
            }
            vector<Node> outputLayer;
            for(int i=0;i<outputSize;i++){
                outputLayer.push_back(Node());
                for(int k=0;k<=hiddenSize[hiddenSize.size()-1];k++){
                    outputLayer[i].weights.push_back(randomFloat());
                }
            }
            model.push_back(outputLayer);
        }

        float cost(vector<float> expected,vector<float> outputs){
            float total=0;
//            cout << "in cost function" << endl;
            for(int i=0;i<expected.size();i++){
                total+=pow(expected[i]-outputs[i],2);
            }
            return total;
        }

        void updateWeights(vector<float> data, float rate){
            vector<float> inputs=data;
            inputs.erase(inputs.end()-1);
            for(int x=0;x<model.size();x++){
                if(x!=0){
                    inputs={};
                    for(int k=0;k<model[x-1].size();k++){
                        inputs.push_back(model[x-1][k].output);
                    }
                }
                for(int y=0;y<model[x].size();y++){
                    for(int k=0;k<inputs.size();k++){
                        model[x][y].weights[k]+=rate*model[x][y].delta*inputs[k];
                    }
                    model[x][y].weights[model[x][y].weights.size()-1]+=rate*model[x][y].delta;
                }
            }
        }

        void trainNet(vector<vector<float>> dataset, float rate, int epoch, float minError,int outputSize){
            float errorSum=-1;
            for(int iter=0;(iter<epoch||epoch==0)&&(errorSum==-1||errorSum>minError||minError==0);iter++){
                errorSum=0;
                for(int set=0;set<dataset.size();set++){
                    vector<float> outputs=forwardPropagate(dataset[set]);
                    vector<float> expected=vector<float>(outputSize);
                    expected[dataset[set][dataset[set].size()-1]]=1;
                    errorSum+=cost(expected,outputs);
                    backwardPropagate(expected);
                    updateWeights(dataset[set],rate);
                }
                cout <<"Epoch: "<<iter<<", Rate: "<<rate <<", Minimum Error: " << minError <<", Error: "<<errorSum<< endl;
            }
        }

        void display(){
            for(int i=0;i<model[0][0].weights.size()-1;i++){
                cout << "(-) ";
            }
            cout << endl;
            for(int x=0;x<model.size();x++){
                for(int y=0;y<model[x].size();y++){
                    cout << "[ ";
                    for(int i=0;i<model[x][y].weights.size();i++){
                        int maximum=model[x][y].weights.size()-1;
                        cout << (i==maximum?"{":"(") <<model[x][y].weights[i] << (i==maximum?"} ":") ");
                    }
                    cout << "] ";
                }
                cout << endl;
                for(int y=0;y<model[x].size();y++){
                    cout << "(-)"<<" ";
                }
                cout << endl;
            }
        }
        Case test(vector<float> testCase){
            vector<float> result=forwardPropagate(testCase);
            int id=0;
            for(int i=1;i<result.size();i++){
                if(result[i]>result[id]){
                    id=i;
                }
            }
            Case output=Case();
            output.outputs=result;
            output.answer=id;
            return output;
        }
};

int main(){
    vector<vector<float>> dataset;
    ifstream datain ("dataset.in");
    ifstream testin ("tests.in");
    ifstream configin ("config.in");
    vector<string> dataIn;
    vector<string> testIn;
    vector<string> configIn;
    vector<int> layerCounts;
    string contents;
    while(getline(datain,contents)) {
        dataIn.push_back(contents);
    }
    while(getline(testin,contents)) {
        testIn.push_back(contents);
    }
    while(getline(configin,contents)) {
        configIn.push_back(contents);
    }
    for(int i=0;i<dataIn.size();i++){
        vector<string> line=split(dataIn[i]," ");
        vector<float> dataCase;
        for(int k=0;k<line.size();k++){
            dataCase.push_back(stof(line[k]));
        }
        dataset.push_back(dataCase);
    }
    Parameters params=Parameters();
    params.epoch=stoi(configIn[0]);
    params.error=stof(configIn[1]);
    params.rate=stof(configIn[2]);
    vector<string> splitLayers=split(configIn[3]," ");
    for(int i=0;i<splitLayers.size();i++){
        layerCounts.push_back(stoi(splitLayers[i]));
    }
    NeuralNet network=NeuralNet(dataset,layerCounts,params);
    for(int i=0;i<testIn.size();i++){
        vector<string> line=split(testIn[i]," ");
        vector<float> testCase;
        for(int k=0;k<line.size();k++){
            testCase.push_back(stof(line[k]));
        }
        Case result=network.test(testCase);
        float expected=testCase[testCase.size()-1];
        string status=network.outputSet[result.answer]==expected?"[OK]":"[NG]";
        cout << status << " Result: " << network.outputSet[result.answer] << ", Expected: " << expected << endl;
    }

    return 0;
}