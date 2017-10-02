#ifndef OKAPI_CHASSISMODEL
#define OKAPI_CHASSISMODEL

#include <array>
#include <initializer_list>
#include <valarray>
#include <API.h>
#include <memory>
#include "control/motor.h"

namespace okapi {
  class ChassisModel {
  public:
    ChassisModel() {}
    virtual ~ChassisModel() = default;
    virtual void driveForward(const int power) = 0;
    virtual void driveVector(const int distPower, const int anglePower) = 0;
    virtual void turnClockwise(const int power) = 0;
    virtual void stop() = 0;
    virtual void tank(const int leftVal, const int rightVal) = 0;
    virtual void arcade(const int verticalVal, const int horizontalVal) = 0;
    virtual std::valarray<int> getEncoderVals() const = 0;
  };

  class ChassisModelParams {
  public:
    ChassisModelParams() {}
    virtual ~ChassisModelParams() = default;

    /**
     * Allocates a new ChassisModel
     * @return shared_ptr pointing to new ChassisModel
     */
    virtual std::shared_ptr<ChassisModel> make() const = 0;
  };

  template<size_t motorsPerSide>
  class SkidSteerModel;

  template<size_t motorsPerSide>
  class SkidSteerModelParams : public ChassisModelParams {
  public:
    SkidSteerModelParams(const std::array<Motor, motorsPerSide * 2>& imotorList, Encoder ileftEnc, Encoder irightEnc):
      motorList(imotorList),
      leftEnc(ileftEnc),
      rightEnc(irightEnc) {}

    virtual ~SkidSteerModelParams() = default;

    std::shared_ptr<ChassisModel> make() const override {
      return std::make_shared<SkidSteerModel<motorsPerSide>>(*this);
    }

    const std::array<Motor, motorsPerSide * 2>& motorList;
    Encoder leftEnc, rightEnc;
  };

  template<size_t motorsPerSide>
  class SkidSteerModel : public ChassisModel {
  public:
    /**
     * Model for a skid steer drive (wheels parallel with robot's direction of
     * motion). When all motors are powered +127, the robot should move forward
     * in a straight line at full speed.
     * @param imotors   Motors in the format: {{left side motors}, {right side motors}}
     * @param ileftEnc  Left side encoder
     * @param irightEnc Right side encoder
     */
    SkidSteerModel(const std::array<Motor, motorsPerSide * 2>& imotorList, const Encoder ileftEnc, const Encoder irightEnc):
      motors(imotorList),
      leftEnc(ileftEnc),
      rightEnc(irightEnc) {}

    SkidSteerModel(const SkidSteerModelParams<motorsPerSide>& iparams):
      motors(iparams.motorList),
      leftEnc(iparams.leftEnc),
      rightEnc(iparams.rightEnc) {}

    SkidSteerModel(const SkidSteerModel<motorsPerSide>& other):
      motors(other.motors),
      leftEnc(other.leftEnc),
      rightEnc(other.rightEnc) {}

    virtual ~SkidSteerModel() { delete &motors; }

    void driveForward(const int power) override {
      for (size_t i = 0; i <= motorsPerSide * 2; i++)
        motors[i].setTS(power);
    }

    void driveVector(const int distPower, const int anglePower) override {
      for (size_t i = 0; i < motorsPerSide; i++)
        motors[i].setTS(distPower + anglePower);
      for (size_t i = motorsPerSide; i < motorsPerSide * 2; i++)
        motors[i].setTS(distPower - anglePower);
    }

    void turnClockwise(const int power) override {
      for (size_t i = 0; i < motorsPerSide; i++)
        motors[i].set(power);
      for (size_t i = motorsPerSide; i < motorsPerSide * 2; i++)
        motors[i].set(-1 * power);
    }

    void stop() override {
      for (size_t i = 0; i <= motorsPerSide * 2; i++)
        motors[i].setTS(0);
    }

    void tank(const int leftVal, const int rightVal) override {
      for (size_t i = 0; i < motorsPerSide; i++)
        motors[i].set(leftVal);
      for (size_t i = motorsPerSide; i < motorsPerSide * 2; i++)
        motors[i].set(rightVal);
    }

    void arcade(const int verticalVal, const int horizontalVal) override {
      for (size_t i = 0; i < motorsPerSide; i++)
        motors[i].set(verticalVal + horizontalVal);
      for (size_t i = motorsPerSide; i < motorsPerSide * 2; i++)
        motors[i].set(verticalVal - horizontalVal);
    }

    std::valarray<int> getEncoderVals() const override {
      return std::valarray<int>{encoderGet(leftEnc), encoderGet(rightEnc)};
    }
  private:
    const std::array<Motor, motorsPerSide * 2> motors;
    const Encoder leftEnc, rightEnc;
  };

  template<size_t motorsPerCorner>
  class XDriveModel;

  template<size_t motorsPerCorner>
  class XDriveModelParams : public ChassisModelParams {
  public:
    XDriveModelParams(const std::array<unsigned char, motorsPerCorner * 4>& imotorList, const Encoder ileftEnc, const Encoder irightEnc):
      motorList(imotorList),
      leftEnc(ileftEnc),
      rightEnc(irightEnc) {}

    virtual ~XDriveModelParams() {}

    std::shared_ptr<ChassisModel> make() const override {
      return std::make_shared<XDriveModel<motorsPerCorner>>(*this);
    }

    const std::array<unsigned char, motorsPerCorner * 4>& motorList;
    const Encoder leftEnc, rightEnc;
  };

  template<size_t motorsPerCorner>
  class XDriveModel : public ChassisModel {
  public:
    /**
     * Model for an x drive (wheels at 45 deg from a skid steer drive). When all
     * motors are powered +127, the robot should move forward in a straight line
     * at full speed.
     * @param imotors Motors in the format: {{top left motors}, {top right motors}, {bottom right motors}, {bottom left motors}}
     */
    XDriveModel(const std::array<unsigned char, motorsPerCorner * 4>& imotorList, const Encoder ileftEnc, const Encoder irightEnc):
      motors(imotorList),
      leftEnc(ileftEnc),
      rightEnc(irightEnc) {}

    XDriveModel(const XDriveModelParams<motorsPerCorner>& iparams):
      motors(iparams.motorList),
      leftEnc(iparams.leftEnc),
      rightEnc(iparams.rightEnc) {}

    XDriveModel(const XDriveModel<motorsPerCorner>& other):
      motors(other.motors),
      leftEnc(other.leftEnc),
      rightEnc(other.rightEnc) {}

    virtual ~XDriveModel() { delete &motors; }

    void driveForward(const int power) override {
      for (size_t i = 0; i < motorsPerCorner * 4; i++)
        motors[i].setTS(power);
    }

    void driveVector(const int distPower, const int anglePower) override {
      for (size_t i = 0; i < motorsPerCorner; i++)
        motors[i].setTS(distPower + anglePower);
      for (size_t i = motorsPerCorner; i < motorsPerCorner * 2; i++)
        motors[i].setTS(distPower - anglePower);
      for (size_t i = motorsPerCorner * 2; i < motorsPerCorner * 3; i++)
        motors[i].setTS(distPower - anglePower);
      for (size_t i = motorsPerCorner * 3; i < motorsPerCorner * 4; i++)
        motors[i].setTS(distPower + anglePower);
    }

    void turnClockwise(const int power) override {
      for (size_t i = 0; i < motorsPerCorner; i++)
        motors[i].setTS(power);
      for (size_t i = motorsPerCorner; i < motorsPerCorner * 2; i++)
        motors[i].setTS(-1 * power);
      for (size_t i = motorsPerCorner * 2; i < motorsPerCorner * 3; i++)
        motors[i].setTS(-1 * power);
      for (size_t i = motorsPerCorner * 3; i < motorsPerCorner * 4; i++)
        motors[i].setTS(power);
    }

    void stop() override {
      for (size_t i = 0; i < motorsPerCorner * 4; i++)
        motors[i].setTS(0);
    }

    void tank(const int leftVal, const int rightVal) override {
      for (size_t i = 0; i < motorsPerCorner; i++)
        motors[i].set(leftVal);
      for (size_t i = motorsPerCorner; i < motorsPerCorner * 2; i++)
        motors[i].set(rightVal);
      for (size_t i = motorsPerCorner * 2; i < motorsPerCorner * 3; i++)
        motors[i].set(rightVal);
      for (size_t i = motorsPerCorner * 3; i < motorsPerCorner * 4; i++)
        motors[i].set(leftVal);
    }

    void arcade(const int verticalVal, const int horizontalVal) override {
      for (size_t i = 0; i < motorsPerCorner; i++)
        motors[i].set(verticalVal + horizontalVal);
      for (size_t i = motorsPerCorner; i < motorsPerCorner * 2; i++)
        motors[i].set(verticalVal - horizontalVal);
      for (size_t i = motorsPerCorner * 2; i < motorsPerCorner * 3; i++)
        motors[i].set(verticalVal - horizontalVal);
      for (size_t i = motorsPerCorner * 3; i < motorsPerCorner * 4; i++)
        motors[i].set(verticalVal + horizontalVal);
    }

    void xArcade(const int verticalVal, const int horizontalVal, const int rotateVal) {
      for (size_t i = 0; i < motorsPerCorner; i++)
        motors[i].set(verticalVal + horizontalVal + rotateVal);
      for (size_t i = motorsPerCorner; i < motorsPerCorner * 2; i++)
        motors[i].set(verticalVal - horizontalVal - rotateVal);
      for (size_t i = motorsPerCorner * 2; i < motorsPerCorner * 3; i++)
        motors[i].set(verticalVal + horizontalVal - rotateVal);
      for (size_t i = motorsPerCorner * 3; i < motorsPerCorner * 4; i++)
        motors[i].set(verticalVal - horizontalVal + rotateVal);
    }

    std::valarray<int> getEncoderVals() const override {
      return std::valarray<int>{encoderGet(leftEnc), encoderGet(rightEnc)};
    }
  private:
    const std::array<unsigned char, motorsPerCorner * 4> motors;
    const Encoder leftEnc, rightEnc;
  };
}

#endif /* end of include guard: OKAPI_CHASSISMODEL */
