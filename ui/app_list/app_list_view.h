// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_APP_LIST_VIEW_H_
#define UI_APP_LIST_APP_LIST_VIEW_H_

#include "base/memory/scoped_ptr.h"
#include "ui/app_list/app_list_export.h"
#include "ui/app_list/apps_grid_view_delegate.h"
#include "ui/app_list/search_box_view_delegate.h"
#include "ui/app_list/search_result_list_view_delegate.h"
#include "ui/views/bubble/bubble_delegate.h"

namespace views {
class Widget;
}

namespace app_list {

class AppListModel;
class AppListItemModel;
class AppListViewDelegate;
class ContentsView;
class PaginationModel;
class SearchBoxView;

// AppListView is the top-level view and controller of app list UI. It creates
// and hosts a AppsGridView and passes AppListModel to it for display.
class APP_LIST_EXPORT AppListView : public views::BubbleDelegateView,
                                    public AppsGridViewDelegate,
                                    public SearchBoxViewDelegate,
                                    public SearchResultListViewDelegate {
 public:
  // Takes ownership of |delegate|.
  explicit AppListView(AppListViewDelegate* delegate);
  virtual ~AppListView();

  // Initializes the widget.
  void InitAsBubble(gfx::NativeView parent,
                    PaginationModel* pagination_model,
                    views::View* anchor,
                    const gfx::Point& anchor_point,
                    views::BubbleBorder::ArrowLocation arrow_location);

  void SetBubbleArrowLocation(
      views::BubbleBorder::ArrowLocation arrow_location);

  void SetAnchorPoint(const gfx::Point& anchor_point);

  void Close();

  void UpdateBounds();

 private:
  // Creates models to use.
  void CreateModel();

  // Overridden from views::WidgetDelegateView:
  virtual views::View* GetInitiallyFocusedView() OVERRIDE;
  virtual bool WidgetHasHitTestMask() const OVERRIDE;
  virtual void GetWidgetHitTestMask(gfx::Path* mask) const OVERRIDE;

  // Overridden from views::View:
  virtual bool OnKeyPressed(const ui::KeyEvent& event) OVERRIDE;

  // Overridden from AppsGridViewDelegate:
  virtual void ActivateApp(AppListItemModel* item, int event_flags) OVERRIDE;

  // Overridden from SearchBoxViewDelegate:
  virtual void QueryChanged(SearchBoxView* sender) OVERRIDE;

  // Overridden from SearchResultListViewDelegate:
  virtual void OpenResult(const SearchResult& result,
                          int event_flags) OVERRIDE;
  virtual void InvokeResultAction(const SearchResult& result,
                                  int action_index,
                                  int event_flags) OVERRIDE;

  // Overridden from views::WidgetObserver:
  virtual void OnWidgetClosing(views::Widget* widget) OVERRIDE;
  virtual void OnWidgetActivationChanged(views::Widget* widget, bool active)
      OVERRIDE;

  scoped_ptr<AppListModel> model_;
  scoped_ptr<AppListViewDelegate> delegate_;

  SearchBoxView* search_box_view_;  // Owned by views hierarchy.
  ContentsView* contents_view_;  // Owned by views hierarchy.

  DISALLOW_COPY_AND_ASSIGN(AppListView);
};

}  // namespace app_list

#endif  // UI_APP_LIST_APP_LIST_VIEW_H_
