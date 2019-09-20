#include "oneflow/core/operator/operator.h"

namespace oneflow {

class ClipBoxesToImageOp final : public Operator {
 public:
  OF_DISALLOW_COPY_AND_MOVE(ClipBoxesToImageOp);
  ClipBoxesToImageOp() = default;
  ~ClipBoxesToImageOp() = default;

  void InitFromOpConf() override;
  const PbMessage& GetCustomizedConf() const override {
    return this->op_conf().clip_boxes_to_image_conf();
  }
  Maybe<void> InferBlobDescs(std::function<BlobDesc*(const std::string&)> GetBlobDesc4BnInOp,
                             const ParallelContext* parallel_ctx) const override;
};

void ClipBoxesToImageOp::InitFromOpConf() {
  CHECK(op_conf().has_clip_boxes_to_image_conf());
  EnrollInputBn("boxes", false);
  EnrollInputBn("image_size", false);
  EnrollOutputBn("out", false);
}

Maybe<void> ClipBoxesToImageOp::InferBlobDescs(
    std::function<BlobDesc*(const std::string&)> GetBlobDesc4BnInOp,
    const ParallelContext* parallel_ctx) const {
  // input: boxes (R, 4)
  const BlobDesc* boxes = GetBlobDesc4BnInOp("boxes");
  CHECK_EQ_OR_RETURN(boxes->shape().NumAxes(), 2);
  CHECK_EQ_OR_RETURN(boxes->shape().At(1), 4);
  // input: image_size (2,)
  const BlobDesc* image_size = GetBlobDesc4BnInOp("image_size");
  CHECK_EQ_OR_RETURN(image_size->shape().NumAxes(), 1);
  CHECK_EQ_OR_RETURN(image_size->shape().At(0), 2);
  CHECK_EQ_OR_RETURN(image_size->data_type(), DataType::kInt32);
  // output: out (R, 4)
  *GetBlobDesc4BnInOp("out") = *boxes;
  return Maybe<void>::Ok();
}

REGISTER_OP(OperatorConf::kClipBoxesToImageConf, ClipBoxesToImageOp);

}  // namespace oneflow