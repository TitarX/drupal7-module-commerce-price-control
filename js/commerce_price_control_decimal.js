jQuery(document).ready(function () {
  jQuery("#edit-submit").confirm({
    text: "",
    title: "Подтвердите изменение количества знаков после запятой",
    confirm: function (button) {
      jQuery("#commerce-price-control-decimal-form").submit();
    },
    cancel: function (button) {
      //
    },
    confirmButton: "Продолжить",
    cancelButton: "Отмена",
    post: true,
    confirmButtonClass: "ts-button",
    cancelButtonClass: "ts-button",
    dialogClass: "modal-dialog modal-lg"
  });
});
