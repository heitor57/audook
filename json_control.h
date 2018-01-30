#ifndef JSON_CONTROL_H
#define JSON_CONTROL_H
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QFile>

const QString frname="speech.json",file_nc = "file_name", progressc = "progress";
double get_progress(QString file_name){
    QFile File(frname);
    File.open(QIODevice::ReadOnly | QIODevice::Text);

    QJsonParseError JsonParseError;
    QJsonDocument JsonDocument = QJsonDocument::fromJson(File.readAll(), &JsonParseError);

    File.close();
    //setup
    QJsonArray array;
    QJsonObject obj_root = JsonDocument.object();
    QJsonArray obj_array = obj_root.value("main").toArray();

    int i;
    for(i = 0;i<obj_array.size();i++){
        if(obj_array.at(i).toObject().value(file_nc).toString() ==file_name){
            return obj_array.at(i).toObject().value(progressc).toDouble();
        }
    }
    return 0;
}
void edit_save(QString file_name,int n_progress){
    QFile File(frname);
    File.open(QIODevice::ReadOnly | QIODevice::Text);

    QJsonParseError JsonParseError;
    QJsonDocument JsonDocument = QJsonDocument::fromJson(File.readAll(), &JsonParseError);

    File.close();
    //setup
    QJsonArray array;
    QJsonObject obj,obj_root = JsonDocument.object();
    QJsonArray obj_array = obj_root.value("main").toArray();

    int i;
    for(i = 0;i<obj_array.size();i++){
        if(obj_array.at(i).toObject().value(file_nc).toString() ==file_name){
            obj_array.removeAt(i);
            obj.insert(file_nc,file_name);
            obj.insert(progressc,n_progress);
            obj_array.push_front(obj);

            //setup
            obj_root.insert("main",obj_array);
            JsonDocument.setObject(obj_root);
            //setup
            File.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
            File.write(JsonDocument.toJson());
            File.close();
            break;
        }
    }


}
bool exist_save(QString file_name){
    QFile File(frname);
    File.open(QIODevice::ReadOnly | QIODevice::Text);

    QJsonParseError JsonParseError;
    QJsonDocument JsonDocument = QJsonDocument::fromJson(File.readAll(), &JsonParseError);

    File.close();
    //setup
    QJsonArray array;
    QJsonObject obj,obj_root = JsonDocument.object();
    QJsonArray obj_array = obj_root.value("main").toArray();

    for(int i = 0;i<obj_array.size();i++){
        if(obj_array.at(i).toObject().value(file_nc).toString() == file_name){
            return true;
        }
    }
    return false;
}
void add_new_save(QString file_name, int progress){
    QFile File(frname);
    File.open(QIODevice::ReadOnly | QIODevice::Text);

    QJsonParseError JsonParseError;
    QJsonDocument JsonDocument = QJsonDocument::fromJson(File.readAll(), &JsonParseError);

    File.close();
    //setup
    QJsonArray array;
    QJsonObject obj,obj_root = JsonDocument.object();
    QJsonArray obj_array = obj_root.value("main").toArray();
    obj_root.remove("main");
    //setup
    obj.insert(file_nc,file_name);
    obj.insert(progressc,progress);
    obj_array.push_back(obj);
    //setup
    obj_root.insert("main",obj_array);
    JsonDocument.setObject(obj_root);
    //setup
    File.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
    File.write(JsonDocument.toJson());
    File.close();
}
#endif // JSON_CONTROL_H
