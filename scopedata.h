#ifndef SCOPEDATA_H
#define SCOPEDATA_H
#include "rigolcomm.h"

#include <QObject>
#include <QFile>

class MainWindow;

class ScopeData : public QObject
{
    Q_OBJECT
public:
    explicit ScopeData(QObject *parent = 0);

protected:
    MainWindow *win;
    bool logiccmd(QString cmd,int chan,int offset,int ch);
    QString stringCmd(const QString &cmd) { com.command(cmd.toLatin1()); return QString(com.buffer); }

public:


   RigolComm com;   // ought to be protected but for now...

    struct rigolwfmcfg
    {
        bool set;
        float hscale;
        float hoffset;
        float vscale[2];
        float voffset[2];
        float srate;
        float deltat;
    } config;

    void setConfig(void);
    int convertbuf(int chan, const QString &cmd, bool raw);


    double *chandata[2];
    int chansize;
    int prepExport(bool c1, bool c2);
    int fillExportBuffer(bool c1, bool c2,bool raw);
    int exportEngine(bool dotime=true, bool c1=true, bool c2=true, bool wheader=true, bool wconfig=true, bool raw=false, QFile *file=NULL);
    int command(const QString &cmd) { return com.command(cmd.toLatin1()); }
    float cmdFloat(const QString &cmd) { return com.cmdFloat(cmd.toLatin1()); }
    float cmdFloatlt(const char *cmd) { return cmdFloat(cmd); }
    bool isChannelDisplayed(int chan);
    void do_wave_plot(bool c1, bool c2);
    void do_export_csv(bool c1, bool c2, bool dotime, bool wheader, bool wconfig, bool raw);
    void do_export_ols(bool c1, bool c2, float thresh);
    void do_export_sigrok(bool c1, bool c2, float thresh);
    void waitForStop(void);
    bool connected(void) { return com.connected(); }
    int close(void) { return com.close(); }
    int trigStatus(void) { com.command(":TRIG:STAT?"); return *com.buffer; }
    QString trigMode(void) { return stringCmd(":TRIG:MODE?");  }
    int unlock(void) { return com.unlock(); }
    int open(QString dev) { return com.open(dev.toLatin1()); }
    QString id(void);
    int stop(void) { return com.command(":STOP"); }
    int run(void) { return com.command(":RUN"); }
    int cmdCharIndex(const QString &cmd,const QString &search,int bpos=0);
    int acqMode(void) { command(":ACQ:MODE?"); return *com.buffer; }
    int average(void) { return (int)cmdFloat(":ACQ:AVER?"); }
    bool isLongMemory(void) { command(":ACQ:MDEP?"); return *com.buffer=='L'; }
    float sampleRate(void) { return cmdFloat(":ACQ:SRAT?"); }
    int numChannels(void) { return 2; }
    int scale(int *decade,int *sign);
    QString bandwidthLimit(int chan) { return stringCmd(":CHAN" + QString::number(chan) + ":BWL?"); }
    bool inverted(int chan) { return logiccmd(":CHAN%d:INV?",chan,1,'N'); }
    QString coupling(int chan) { logiccmd(":CHAN%d:COUP?",chan,0,'x'); return QString(com.buffer); }
    bool force(void) { return command(":TFOR"); }
    bool trig50(void) { return command(":TLHA"); }
    QString triggerSource(const QString &mode);
    float triggerHoldUs(void) { return cmdFloat(":TRIG:HOLD?")*1000000.0f; }
    QString trigCoupling(const QString &mode) { return stringCmd(":TRIG:COUP?"); }
    QString edgeSlope(void) { return stringCmd(":TRIG:EDGE:SLOP?"); }
    QString trigPulseMode(void) { return stringCmd(":TRIG:PULS:WHEN?");  }
    QString sweep(const QString &mode) { return stringCmd(":TRIG:SWE?"); }
    QString trigSlopeMode(void) { return stringCmd(":TRIG:SLOP:WHEN?"); }
    QString trigSlopeWin(void) { return stringCmd(":TRIG:SLOP:WIND?"); }
    QString mathOp(void) { return stringCmd(":CALC:MODE?");   }
    bool setAcqTNormal(void) { return command(":ACQ:TYPE NORM"); }
    bool setAcqTAverage(void) { return command(":ACQ:TYPE AVER"); }
    bool setAcqTPeak(void) { return command(":ACQ:TYPE PEAK"); }
    bool setAcqModeRtim(void) { return command(":ACQ:MODE RTIM"); }
    bool setAcqModeEtim(void) { return command(":ACQ:MODE ETIM"); }
    bool setAcqAverage(int n) { return command(":ACQ:AVER "+QString::number(n)); }
    bool setAcqMemNorm(void) { return command(":ACQ:MDEP 14000");  }
    bool setAcqMemLong(void) { return command(":ACQ:MDEP 56000000"); }
    bool setTimeScale(const QString &s) { return command(":TIM:SCAL "+s); }
    bool setTimeOffsetUs(float f) { return command(":TIM:OFFS "+QString::number(f/1000000.0f)); }
    bool setChanDisp(int chan,bool state) { return command(QString(":CHAN")+QString::number(chan)+":DISP "+ (state?"ON":"OFF")); }
    bool setChanBWL(int chan, QString state) { return command(QString(":CHAN")+QString::number(chan)+":BWL "+ state); }
    bool setChanInvert(int chan,bool state) { return command(QString(":CHAN")+QString::number(chan)+":INV "+ (state?"ON":"OFF")); }
    bool setChanProbe(int chan,int x) { return command(QString(":CHAN")+QString::number(chan)+":PROB "+QString::number(x)); }
    bool setChanCouple(int chan,const QString &acdc) { return command(QString(":CHAN")+QString::number(chan)+":COUP "+acdc); }
    bool setChanOffset(int chan,float f) { return command(":CHAN"+QString::number(chan)+":OFFS "+QString::number(f)); }
    bool setChanScale(int chan, const QString &v) { return command(":CHAN" +QString::number(chan)+":SCAL "+v); }
    bool setTrigMode(const QString &mod) { return command(":TRIG:MODE "+mod); }
    bool setTrigLevel(const QString &mod, float f) { return command(":TRIG:" + mod + ":LEV "+QString::number(f)); }
    bool setSweep(const QString &mod,const QString state) { return command(":TRIG:SWE "+state); }
    bool setTrigSource(const QString &mod,const QString state) { return command(":TRIG:"+mod+":SOUR "+state); }
    bool setTrigHoldUs(float f) { return command(":TRIG:HOLD"+QString::number(f/1000000.0f,'f')); }
    bool setTrigCouple(const QString &mod,const QString &acdc) { return command(":TRIG:COUP "+acdc); }
    bool setTrigEdgeSlope(QString slope) { return command(QString(":TRIG:EDGE:SLOP ") + slope); }
    bool setTrigPulseUpperWidthUs(float f) { return command(":TRIG:PULS:UWID "+QString::number(f/1000000.0f)); }
    bool setTrigPulseLowerWidthUs(float f) { return command(":TRIG:PULS:LWID "+QString::number(f/1000000.0f)); }
    bool setTrigPulseMode(const QString &mod) { return command(":TRIG:PULS:WHEN "+mod); }
    bool setMathOper(const QString &op) { return command(":CALC:MODE "+op); }
    bool setTrigSlopeMode(const QString &mod) { return command(":TRIG:SLOP:WHEN "+mod); }
    bool setTrigSlopeWindow(const QString &mod) { return command(":TRIG:SLOP:WIND "+mod); }
    bool setTrigSlopeTimeUs(float f) { return command(":TRIG:SLOP:TIME "+QString::number(f/1000000.0f)); }
    bool setTrigSlopeLevA(float f)  { return command(":TRIG:SLOP:ALEV "+QString::number(f)); }
    bool setTrigSlopeLevB(float f)  { return command(":TRIG:SLOP:BLEV "+QString::number(f)); }

signals:

public slots:



};

#endif // SCOPEDATA_H
